#include <SPI.h>
#include <Ethernet.h>

int sensorpin=4;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//MAC address
IPAddress ip(192,168,137,20);            // ip in lan (that's what you need to use in your browser. ("192.168.137.20")               
EthernetServer server(80);               //server port   

int count=0;
int rate=10;
int bill=0;
  
String readString;

void setup()
{
 
  Serial.begin(9600);
  pinMode(sensorpin,INPUT);
  Ethernet.begin(mac, ip);
  server.begin();  
  Serial.println("AUTOMATED ENERGYMETER");
  Serial.println("BILL:Rs.0        METER:0 Units");
}

void loop() 
{
  // Create a client connection
  if(digitalRead(sensorpin))
      {
              count++;  
              if(count%4==0)
              {
              if(count<200) 
                bill=bill+rate; 
              else
                bill=bill+rate+5;
              }
            
              Serial.print("BILL:Rs.");  
              Serial.print(bill);  
              Serial.print("        METER:"); 
              Serial.print(count/4);  
              Serial.println(" Units");   
              delay(100);
  }
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {   
      
      if (client.available()) 
      {
          char c = client.read(); 
         //read char by char HTTP request
         if (readString.length() < 100) 
         {
          //store characters to string
          readString += c;
         }

         //if HTTP request has ended
         if (c == '\n') 
         {              
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Energy Meter Project</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<br />");  
           client.println("<H2>SMART ENERGY METER</H2>");
           
           client.print("<b>Meter Reading:</b>");
           client.print(count/4);
           client.println(" Units");
           client.println("<br />");
           client.println("<br />");
           client.print("<t><b>BILL:</b>Rs.</t>");
           client.println(bill);
           
           client.println("<br />");
           client.println("<br />");  
           client.println("<a href=\"/?button1on\"\">PRINT</a>");
           client.println("<br />");     
           client.println("</BODY>");
           client.println("<head>");
           client.println("<meta http-equiv='refresh' content='1'>");
           client.println("</head>");
           client.println("</HTML>");
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?button1on") >0)
           {
               digitalWrite(13, HIGH);
           }
           //clearing string for next read
            readString="";                         
         }
       }
    }
  }
}

