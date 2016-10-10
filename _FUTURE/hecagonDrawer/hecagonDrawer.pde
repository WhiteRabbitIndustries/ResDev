import processing.pdf.*;


// 100 pixels to 3.52cm. 
int hexRadius = 6;
int hexHeight = hexRadius*2;
void setup() {
  size(100, 100, PDF, "filename.pdf");
}


int hexShell = 4;
int ledsize = 1+(hexShell-1)*6;
int columnLength = 2*hexShell +1;
int ledCount = hexShell

void draw() {
  // Draw something good here
  line(0, 0, width/2, height);

  // Exit the program 
  println("Finished.");
  exit();
}