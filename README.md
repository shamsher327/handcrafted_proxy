# Handcrafted Proxy Or Reverse Proxy server
 
 - **Availability**: dev-release
 - **Current Version**: 7.x-1.0
 - **Dependencies**: `services`
 
 ## About
 This module is designed for sending reverse proxy web request. Instead of accessing direct any 
 web server, we can use that partially like A<->B<->C . A system is interacting with C but not
 directly. A system will access B and B will get or manipulate C server req/response. In this module
 we are using flexible middleware layer to manipulate req/response format. These are the features
 are included in this module.
 
 - Add Deployment URL.
 - Add Endpoint.
 - Set Target URL.
 - Set format of response like json,array and xml.
 - Add/Save all end points target.
 - Select request method GET/POST.
 - Send header information.
 
 https://github.com/shamsher327/handcrafted_proxy/blob/master/Handcrafted%20Reverse%20Proxy%20Access%20%20%20DRUPAL%207.png
