#ifndef ANIM5_SERVER
#define ANIM5_SERVER

#include <M5Stack.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html lang="en">
    <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <style>
        body {background-color: #FFFF99; margin:25px; font-family: Helvetica, Arial, sans-serif;}
        h2   {color: #404040; top-margin: 5px;}
        p    {color: black;}
        input[type=button], input[type=submit], input[type=reset] {
            background-color: #0066CC;
            border: none;
            color: white;
            padding: 10px 16px;
            border-radius: 5px;
            cursor: pointer;
            margin-left: 15px;
        }
        .files{color: #606060;}
        .data {display: none;}
        .upload-label {
            background-color: #E0e0e0;
            border: none;
            display: inline-block;
            padding: 6px 10px;
            cursor: pointer;
            border-radius: 4px;
            font-size:14px;
        }
    </style>
    </head>
    <body>
    <h1>Animate M5 Stack Server</h1>
    <h2>SD Memory Information</h2>
    <p><b>Total Storage:</b> %MEM_TOTAL%</p>
    <p><b>Free Storage:</b> %MEM_FREE%</p>
    <p><b>Used Storage:</b> %MEM_USED%</p>
    <h2>Upload files</h2>
    <p>After clicking upload, it will take some time for the file to upload and then be written to SD, there is no indicator that the upload began.  Please be patient.</p>
    <p>Once uploaded the page will refresh and the newly uploaded file will appear in the file list.</p>
    <p>Don't use spaces in the filename, otherwise it won't work</p>
    <form method="POST" action="/upload" enctype="multipart/form-data">
      <label class="upload-label">Select one file<input type="file" name="data" class="data"/></label>
      <input type="submit" name="upload" value="Upload" title="Upload File">
    </form>
    <h2>SD directory</h2>
    <p>Current files in memory:</p>
    <p class="files">%FILES%</p>
    </body>
    </html>
)rawliteral";



void handleUpload(AsyncWebServerRequest *request, 
    String filename, size_t index, uint8_t *data, 
    size_t len, bool final);

String humanReadableSize(uint32_t bytes);

String listFiles();

String processor(const String& var);

void configureWebServer(AsyncWebServer *server);

#endif