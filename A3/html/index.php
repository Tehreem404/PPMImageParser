<?php include 'config.php'; ?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <ul>
      <li><a href="fileselector.php">Select File</a></li>
      <li><a onclick="window.open('', '_self', ''); window.close();">Exit</a></li>
    </ul>
    <meta charset="utf-8">
    <title>WebGL Demo</title>
    <link rel="stylesheet" href="css/webgl.css?v=<?=$version?>" type="text/css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js"
    integrity="sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ=="
    crossorigin="anonymous" defer>
    </script>
    <script src="js/webgl-demo.js" defer></script>
    <script src="js/loaddata.js" ></script>
  </head>
  <body style="text-align:center;">
    <canvas id="glcanvas" width="640" height="480"></canvas>
  </body>
  <link rel="stylesheet" href="css/index.css">
</html>
