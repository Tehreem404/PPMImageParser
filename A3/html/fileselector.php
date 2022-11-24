<?php include 'config.php'; ?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <ul>
      <li><a href="index.html">Back</a></li>
      <li><a onclick="window.open('', '_self', ''); window.close();">Exit</a></li>
    </ul>
  </head>
  <body>
    <?php
     echo "<b>hello</b>";
     ?>
  </body>
  <link rel="stylesheet" href="css/index.css?v=<?=$version?>">
</html>