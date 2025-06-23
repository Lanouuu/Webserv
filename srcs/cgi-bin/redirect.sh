#!/bin/sh

echo "HTTP/1.1 302 Found"
echo "Location: https://www.youtube.com/"
echo "Content-Type: text/html"
echo ""

echo "<html><body>Redirection en cours...</body></html>"