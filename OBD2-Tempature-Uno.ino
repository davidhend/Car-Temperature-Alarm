//Declare Serial Read variables
int CmdCount=1;
byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
long DisplayValue;
String SentMessage="";
int ByteCount=0;
long A;
int B;
int WorkingVal;
String WorkingString="";

void setup()
{
  /* ELM327 OBD2 Serial Interface  */ 
  Serial.begin(38400); 

  /* attach speaker to pin 12 */
  pinMode(12, OUTPUT);
  /* set speaker to low  */
  digitalWrite(12, LOW);

  /* Allow the ELM327 to finish initalizing */
  delay(4000);

  Retry:

  delay(500);
  
  //Send a test message to see if the ELM327 Chip is responding
  SentMessage = "ATI";
  Serial.println("ATI");
  delay(500);
  ReadData();
  
  if (BuildINString.substring(1,7)=="ELM327")
    {
      delay(1500);
    }
    else
    {
      delay(1500);
      goto Retry;
    }
  
  delay(1500);

  
}


void loop()
{
  
  SentMessage = "01 05";
  Serial.println("01 05");
  delay(300);
  ReadData();
  
  int tempature = DisplayString.toInt();
  
  if(tempature >= 90){
    digitalWrite(12, HIGH);
  }
  
  delay(1000);
  digitalWrite(12, LOW);   
  
}

//Read Data and act accordingly
void ReadData()
{
  BuildINString="";  
  while(Serial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = Serial.read();
    inChar=char(inData);
    BuildINString = BuildINString + inChar;
  }
  
  //if(BuildINString!=""){Serial.print(BuildINString);}
  BuildINString.replace(SentMessage,"");
  BuildINString.replace(">","");
  BuildINString.replace("OK","");
  BuildINString.replace("STOPPED","");
  BuildINString.replace("SEARCHING","");
  BuildINString.replace("NO DATA","");
  BuildINString.replace("?","");
  BuildINString.replace(",","");
  //Serial.print(BuildINString);
  
  //Check which OBD Command was sent and calculate VALUE 
  //Calculate RPM I.E Returned bytes wil be 41 0C 1B E0 
  if (SentMessage=="01 0C")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
    WorkingString = BuildINString.substring(11,13);
    B = strtol(WorkingString.c_str(),NULL,0);
   
    DisplayValue = ((A * 256)+B)/4;
    DisplayString = String(DisplayValue) + " rpm";

  }
  
  //Calculate Vehicle speed I.E Returned bytes wil be 41 0C 1B E0 
  if (SentMessage=="01 0D")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);

    DisplayValue = A;
    DisplayString = String(DisplayValue) + " km/h";

  }
  
  //Coolant Temp 
  if (SentMessage=="01 05")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
	
    DisplayValue = A;
    DisplayString = String(DisplayValue) + " C";

  }
  
  //IAT Temp 
  if (SentMessage=="01 0F")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
 
    DisplayValue = A;
    DisplayString = String(DisplayValue) + " C";
  }
  
  //Air flow Rate
  if (SentMessage=="01 10")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
    WorkingString = BuildINString.substring(11,13);
    B = strtol(WorkingString.c_str(),NULL,0);
   
    DisplayValue = ((A * 256)+B)/100;
    DisplayString = String(DisplayValue) + " g/s";
  }
  
  //Ambient Temp 
  if (SentMessage=="01 46")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
		
    DisplayValue = A;
    DisplayString = String(DisplayValue) + " C";
  }
  
  //Throttle position 
  if (SentMessage=="01 11")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
	
    DisplayValue = A;
    DisplayString = String(DisplayValue) + " %";
  }

  //Barometric pressure
  if (SentMessage=="01 33")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);

    DisplayValue = A;
    DisplayString = String(DisplayValue) + " kpa";
  }  
  
}

