<?php
/**
 * @file
 * Configuration settings for this module.
 */

/**
 * Implements form callback.
 *
 * Form for configuring all target values.
 */
function handcrafted_proxy_config($form, &$form_state) {
  global $base_url;

  $form['deployment_request'] = [
    '#title'         => t('Deployment URL'),
    '#type'          => 'textfield',
    '#description'   => 'Deployement Request should start with ' . $base_url . ' /api/endpoints',
    '#default_value' => variable_get('deployment_request',
      $base_url . '/api/google_books'),
  ];

  $form['endpoint'] = [
    '#title'         => t('Endpoint'),
    '#type'          => 'textfield',
    '#description'   => 'Endpoint request value',
    '#default_value' => variable_get('endpoint', '/google_books'),
  ];

  $form['target'] = [
    '#title'         => t('Target URL'),
    '#type'          => 'textfield',
    '#description'   => 'Target value URL Access',
    '#default_value' => variable_get('target', 'www.google.books/api/v1/value'),
  ];

  $setting_formatter['formatters'] = [
    'json'    => 'json',
    'bencode' => 'bencode',
    'jsonp'   => 'jsonp',
    'array'   => 'array',
    'php'     => 'php',
    'xml'     => 'xml',
  ];

  $form['formatters'] = [
    '#type'          => 'radios',
    '#title'         => t('Response formatters'),
    '#required'      => TRUE,
    '#options'       => $setting_formatter['formatters'],
    '#tree'          => TRUE,
    '#description'   => t('Select the response formats you want to enable for the rest server.'),
    '#default_value' => variable_get('formatters', 'json'),
  ];

  $form['formatters_method_type'] = [
    '#type'          => 'radios',
    '#title'         => t('Response formatters'),
    '#required'      => TRUE,
    '#options'       => [
      'get'  => t('Get'),
      'post' => t('Post'),
    ],
    '#description'   => t('Select the response formats you want to enable for the rest server.'),
    '#default_value' => variable_get('formatters_method_type', 'get'),
  ];

  $form['header'] = [
    '#type'          => 'textarea',
    '#title'         => t('Post Service Header'),
    '#description'   => t('Give Header information in json format'),
    '#states'        => [
      'visible' => [
        ':input[name="formatters_method_type"]' => ['value' => 'post'],
      ],
    ],
    '#default_value' => variable_get('header', '{"username":"shamhsher"}'),
  ];

  // Get saved values from db in to table format.
  $form['table'] = [
    '#markup'      => handcrafted_target_and_endpoints(),
    '#description' => t('List of save target and endpoints.'),
  ];

  $form['submit'] = [
    '#value' => t('Save'),
    '#type'  => 'submit',
  ];

  return $form;
}

/**
 * Process form submissions.
 */
function handcrafted_proxy_config_submit($form, &$form_state) {
  $form_state['render_table'] = TRUE;
  $form_state['rebuild'] = TRUE;

  // All form values.
  $form_values = $form_state['values'];

  // Store Access Points.
  db_merge('end_points_and_target')
    ->key(['endpoint' => $form_values['endpoint']])
    ->key(['target' => $form_values['target']])
    ->key(['deployment' => $form_values['deployment_request']])
    ->fields([
      'deployment'      => $form_values['deployment_request'],
      'endpoint'        => $form_values['endpoint'],
      'target'          => $form_values['target'],
      'response_format' => $form_values['formatters'],
      'method_type'     => $form_values['formatters_method_type'],
    ])
    ->execute();
}

/**
 * Provide a single block from the administration menu as a page.
 *
 * This function is often a destination for these blocks.
 * For example, 'admin/structure/types' needs to have a destination to be valid
 * in the Drupal menu system, but too much information there might be
 * hidden, so we supply the contents of the block.
 *
 * @return
 *   The output HTML.
 */
function handcrafted_proxy_admin_menu_block_page() {
  $item = menu_get_item();
  if ($content = system_admin_menu_block($item)) {
    $output = theme('admin_block_content', ['content' => $content]);
  }
  else {
    $output = t('You do not have any administrative items.');
  }
  return $output;
}

/**
 * Create table format using hook_theme_table.
 *
 * @return string
 */
function handcrafted_target_and_endpoints() {
  $output = '';
  // display current clients
  $sql = db_select('end_points_and_target', 'end')->fields('end', [
    'id',
    'deployment',
    'endpoint',
    'target',
    'method_type',
    'response_format',
  ])->execute();

  /* with table */
  $header = [
    'ID',
    'Deployment',
    'Endpoinet',
    'Target',
    'Request Type',
    'Response Format',
    'Action',
    'Send',
  ];
  $rows = [];
  $count = 0;
  // Looping for filling the table rows
  while ($record = $sql->fetchAssoc()) {
    // Fill the table rows
    $rows[$count++] = [
      $record['id'],
      $record['deployment'],
      $record['endpoint'],
      $record['target'],
      $record['method_type'],
      $record['response_format'],
      l('Delete', '/delete/end_point/' . $record['id']),
      l('Get Data',
        '/send/web_request' . $record['endpoint'] . '/' . $record['response_format']),
    ];
  }

  // Output of table with the paging
  $output .= theme_table([
    "header"     => $header,
    "rows"       => $rows,
    "attributes" => [],
    "sticky"     => TRUE,
    // Table header will be sticky
    "caption"    => "",
    "colgroups"  => [],
    "empty"      => t("Table has no data!")
    // The message to be displayed if table is empty
  ]);

  return $output;
}
