#!/bin/sh
echo "HTTP/1.1 200 OK"
echo "Content-Type: text/html";
echo "";

tmp=`/bin/date`
sleep 5
cat << EOF

<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="/www/style.css">
        <link rel="icon" type="image/x-icon" href="/images/icon/favicon.ico">
        <title>Webserv</title>
    </head>
    <body>
        <header>
            <div>
                <h1><a href="/www/index.html">Webserv</a></h1>
            </div>
        </header>
        <main>
            <div class="text_btn">
                <h1>The current date on the server is</h1>
                <h3>$tmp</h3>
                <a href="/www/index.html" class="btn-home">Go to home page</a>
            </div>
        </main>
    </body>
    <footer>
        <p>© 2025 Webserv, Inc. aciezadl - jaung - ade-fran</p>
    </footer>
</html>

EOF