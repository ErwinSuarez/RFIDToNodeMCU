<html>
  <head>
    <title>Raspberry Pi3 Server</title>
    <style>
      h1, h2, p { text-aligned: center; }
    </stlye>
  </head>
  
  <?php
    $uid = $_GET["uid"];
    $room = $_GET["roomID"];
    $todayString = date("l : jS \of F Y h:i:s A")."<br>";
    
    $hostname = "localhost";
    $username = "root";
    $password = "raspberry";
    $dbname = "eventDBv2";  
  ?>
  
  <body>
    <h1>Welcome to Raspberry Pi</h1>
    
    <h2><?php echo $todayString ?></h2>
    
    <h2>Card UID: <?php echo $uid ?></h2>
    <h2>Room: <?php echo $room ?></h2>
    
    <p>This si still in development</p>
  </body>
</html>

