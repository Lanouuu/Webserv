#!/bin/sh
echo "HTTP/1.1 200 OK"
echo "Content-Type: text/html";
echo "";

tmp=`/bin/date`

cat << EOF

<!DOCTYPE html>
<head><title>Script Cgi</title></head>
<body>

<center>

<h1>La date courante sur le serveur est</h1>
<p>$tmp</p>

</center>

</body>
</html>

EOF