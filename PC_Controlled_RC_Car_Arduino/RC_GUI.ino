#include <VirtualWire.h>
public class GUI extends JPanel implements ActionListener, KeyListener {
    public JButton bFront, bBack, bLeft, bRight; //Define the buttons
    StyledDocument instructionsText; //This is used for the instructions
    public JTextPane instructions; //Also used for instructions
    public GUI() {
    setLayout(new GridBagLayout()); //Set the layout to Grid Bag
    this.setPreferredSize(new Dimension(400,400));//Set the size of the window
    GridBagConstraints c = new GridBagConstraints();
    c.fill = GridBagConstraints.BOTH;
    c.gridx = 0; //This will be the position of the instructions
    c.gridy = 0;
    c.gridwidth = 4; //The instructions will span 4 tiles
    //Create the instructions text
    StyledDocument instructionsText = new DefaultStyledDocument();
    Style defaultStyle = instructionsText.getStyle(StyleContext.DEFAULT_STYLE);
    StyleConstants.setAlignment(defaultStyle, StyleConstants.ALIGN_CENTER);
    try {
    instructionsText.insertString(0, "Control the car using the keyboard.\n Change the key bindings by clicking the buttons below.", null);
    } catch (BadLocationException e) {
    e.printStackTrace();
}
     //add the text to the pane
 instructions = new JTextPane(instructionsText);
 instructions.setFocusable(false);
 //add the pane to the window
 add(instructions,c);
 //Create the forward button (same for others below)
 bFront = new JButton("Forward key: " + KeyEvent.getKeyText(frontKey));
 bFront.setActionCommand("forward");
 bFront.addActionListener(this);
 bFront.setFocusable(false);
 //Set position of the forward button (same for others below)
 c.gridwidth = 1;
 c.gridx = 1;
 c.gridy = 1;
 add(bFront,c);
 bBack = new JButton("Backward key: " + KeyEvent.getKeyText(backKey));
 bBack.setActionCommand("backward");
 bBack.addActionListener(this);
 bBack.setFocusable(false);
 c.gridx = 1;
 c.gridy = 4;
 add(bBack,c);
 bLeft = new JButton("Left key: " + KeyEvent.getKeyText(leftKey));
 bLeft.setActionCommand("left");
 bLeft.addActionListener(this);
 bLeft.setFocusable(false);
 c.gridx = 0;
 c.gridy = 3;
 add(bLeft,c);
 bRight = new JButton("Right key: "+ KeyEvent.getKeyText(rightKey));
 bRight.setActionCommand("right");
 bRight.addActionListener(this);
 bRight.setFocusable(false);
 c.gridx = 3;
 c.gridy = 3;
 add(bRight,c);
 setFocusable(true);
 addKeyListener(this);
}
}
 public static void main(String[] args) {
 // TODO Auto-generated method stub
 //Create and show the GUI
 javax.swing.SwingUtilities.invokeLater(new Runnable() {
 public void run() {
 createAndShowGUI();
 }
 });
}
 private static void createAndShowGUI() {
 //Create and set up the window.
 JFrame frame = new JFrame("Arduino RC Car Controller");
 frame.setLocationRelativeTo(null);
 //Tell the program to exit when the GUI is closed
 frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
 final GUI gui = new GUI(); //create the GUI
 gui.setOpaque(true); //Set it to visible
 frame.setContentPane(gui); //attach it to the Jframe
 //Display the window.
 frame.pack();
 frame.setVisible(true);
 }
 @Override
//If a button is clicked tell the program to listen for the next key pressed
public void actionPerformed(ActionEvent e) {
if(e.getActionCommand().equals("forward")) {
 listenKey = 1;
} else if(e.getActionCommand().equals("backward")) {
 listenKey = 2;
} else if(e.getActionCommand().equals("left")) {
 listenKey = 3;
} else if(e.getActionCommand().equals("right")) {
 listenKey = 4;
}
}
@Override
public void keyPressed(KeyEvent e) {
//When a key is pressed
//if not listening for a key change the color of the corresponding button
//and save the state of the key
if(listenKey <= 0)
 if(e.getKeyCode() == frontKey) {
 bFront.setBackground(Color.yellow);
 frontKeyPressed = true;
 } else if(e.getKeyCode() == backKey) {
 bBack.setBackground(Color.yellow);
 backKeyPressed = true;
 } else if(e.getKeyCode() == leftKey) {
 bLeft.setBackground(Color.yellow);
 leftKeyPressed = true;
 } else if(e.getKeyCode() == rightKey) {
 bRight.setBackground(Color.yellow);
 rightKeyPressed = true;
 }
}
@Override
public void keyReleased(KeyEvent e) {
//When a key is released
//If we are waiting for a key
switch(listenKey) {
 //Set the control for the requred button
 case 1:
 frontKey = e.getKeyCode();
 bFront.setText("Forward key: " + KeyEvent.getKeyText(frontKey));
 listenKey = 0;
 break;
 case 2:
 backKey = e.getKeyCode();
 bBack.setText("Backward key: " + KeyEvent.getKeyText(backKey));
 listenKey = 0;
 break;
 case 3:
 leftKey = e.getKeyCode();
 bLeft.setText("Left key: " + KeyEvent.getKeyText(leftKey));
 listenKey = 0;
 break;
 case 4:
 rightKey = e.getKeyCode();
 bRight.setText("Right key: " + KeyEvent.getKeyText(rightKey));
 listenKey = 0;
 break;
 //Otherwise return the button to normal
 default:
 if(e.getKeyCode() == frontKey) {
 bFront.setBackground(null);
 frontKeyPressed = false;
 } else if(e.getKeyCode() == backKey) {
 bBack.setBackground(null);
 backKeyPressed = false;
 } else if(e.getKeyCode() == leftKey) {
 bLeft.setBackground(null);
 leftKeyPressed = false;
 } else if(e.getKeyCode() == rightKey) {
 bRight.setBackground(null);
 rightKeyPressed = false;
 }
 break;
}
}
Thread t=new Thread() {
public void run() {
 //Attempt to initialize the serial connection
 while(!gui.initialize()) {
 System.out.print("Waiting one second...");
 try {Thread.sleep(1000);} catch (InterruptedException ie) {}
 }
 //Send data every 100ms
 while(true) {
 gui.sendData();
 try {Thread.sleep(100);} catch (InterruptedException ie) {}
 }
}
};
t.start();
System.out.println("Started");
public void sendData(){
try {
 //If forward key is pressed send forward etc...
 if(frontKeyPressed){
 output.write(FORWARD);
 } else if(backKeyPressed){
 output.write(BACKWARD);
 } else {
 output.write(STOP);
 }
 if(leftKeyPressed){
 //If both left and right are pressed the car will go straight
 if(rightKeyPressed)
 output.write(STRAIGHT);
 else
 output.write(LEFT);
 } else if (rightKeyPressed){
 output.write(RIGHT);
 } else {
 output.write(STRAIGHT);
 }
} catch(Exception e) {
 e.printStackTrace();
}
}
