/* Plotting and manipulating graphics on 24x32 daisy chained LED matrixes
by Benjamin Gibbs
date started 2021/12/30

manicminer_on_24x32_led_matrix

off shoot from
led_matrix_graphics_engine_v1_4
off shoot from 
v3_2_24x32_led_matrix_graphics_plotting project

## setup info ##
uses Max7219 LED 3 lots of 8 x 64 modules daisy chained
Use external power as the displays will use a lot of current
CLK - PIN 13
CS - PIN 10
DIN - PIN 11
module 0 = bottom left; top right is 11 in this instance
col 0 is at the top
MSB is on the left of the module
One joystick for moving the character left, right, and the screen up and down
Using another joystick just for the button
Buzzer in pin 6

## aims ##
Improve method of displaying graphics on 3 daisy chained 8x32 MAX7219 led matrixes

## Updates - newest first ##

v6_0 2022/01/26
Collision detection for trumpet face added and tweaked.
Commented out the key counter reset when willy dies as its rock hard to play
I managed to jump over trumpet face once !

v6_0 2022/01/25
Very quickly added the intro
Need to work out how to loop it then stop when joystick button pressed


v5_9 2022/01/25
Added collision detection for the drop floors and its not perfect but well enough to play and fall through
Added collision detection for gate once all keys are collected
added a sound effect when you 'win'
Dying will reset all the dropfloors in the drop floor array and put them in starting positions in level map

v5_8 2022/01/25
Updated show level to display dropfloors from the dropfloor array
All showing in correct place

v5_7 2022/01/23
Setting up ready for dropfloors
Excluded from the showlevel function
Excluded from feet collision detection
Created function to populate the dropfloorarray of strucure info

v5_6 2022/01/23
Playing around with a drop floor struct that willy can slowly fall through
it works but needs a lot of thought and tweaking to put it in the game

v5_5 2022/01/21
Added a buzzer and using tone library to handle it

v5_4 2022/01/21
Added a key counter
made a playgame function to group everything nicer
made a finish game fun that flashes the gate if all keys are collected

v5_3 2022/01/21
Changed mapping for x locations to use size of so level size can be adjusted and it will automatically accomadate this
Added an extra left and right collision check when willy is stood on a conveyer otherwise it doesn't detect anything when hes on it
Added an invert sprite function so the gate can flash when keys are collected

v5_2 2022/01/19
Removed comments from joystick section so you can move the 'screen' up and down.
Added bonus that will retain that level so you move while seeing what's underneath you
Added the animated conveyor belt back in and changed the sprite to make it look more belt like

v5_1 2022/01/19
Changed jumping variables (2 up, 1 down, and height) so that you can actually jump bushes
Changed collision detection to use a reference so that keys can be collected
Added it so conveyer belt forces willy to look left
IT DOESN'T DETECT OTHER OBJECTS ON CONVEYER BELT 
ADDED A QUEUE POINT TO JUMP FROM BUT WILLY WILL LAND ON IT

v5_0 2022/01/19
Added size of array calculation to anything that uses map function to make life easier
Changed the yscreen map function to include a -1 as that seems to remove glitches
added a row of bricks to top of level otherwise it showsrandom memory locations when you jump on top ledge
Increased the level height variable by 8 to accomidate this

v4_9 2022/01/17
ABANDONED DUE TO INTRODUCING MORE ISSUES THAN IT FIXED

v4_8 2022/01/16
Left and right side collisions work for brick and bushes
I need a seperate jump button as its too hard to control walking and jumping

v4_7 2022/01/16
collision deection is sort of working for brick but it refuses to detect the conveyor belt above
detects it below fine though !?
Need to add a layer of bricks on the top of the level
Fixed an issue I didnt realise I had in collision detection feet function
Fixed a mistake with a = instead of == so Willy can die now

v4_6 2022/01/16
Joystick button jumps
Increased Willy's mass so he falls quicker

v4_5 2022/01/15
Got jumping function working
Collision detection x width reduced

v4_4 2022/01/14
Updated display level funcion so that it can handle the y axis
rewrote the collision detection which needs further testing as it sort of works
jumping function needs a rewrite

v4_3 2022/01/13
Made an array map for level instead of using a co-ordinates array.
Consumes more memory but should make collision detection easier
Having to use the map function more than I like
Need to sort out y part 

v4_2 2022/01/12
Colision detection is a bust
You can keep jumping up, commically.
If the objeccts are too far down the list then willy falls through them
The camera and him do float to the ground nicely
choices are a big map array or limit the object array check range

v4_1 2022/01/12
collision detection goes through objects array and checks below willy to see if hes landed on something then changes his and y screen approiately
unfortuanly it doesnt detect if he walks off an edge

v4_0 2022/01/12
added to willyInfo to add a gravity column...bascially where to stop gravty being applied
Will use this for collision detectiom
Jumping works

v3_9 2022/01/11
This project has got to the point where I just want it finished so I'm  doing things to fit with previous code inside of writing new functions
example, using lots of single arrays for moving sprites instead of 2d or 3d arrays
Applied gravity to willy sprite and yscreen 

v3_8 2022/01/11
Made the screen move left and right depending which way willy faces NEEDS WORK WHEN AT RIGHT EDGE OF LEVEL

v3_7 2022/01/11
Added all the info to display Willy and handle his attributes

v3_6 2022/01/11
Added conveyer belt sprite, locations, and reverse the sprite in animated object loop to make it look like they move

v3_5 2022/01/10
Lots of work on trumpet face
added screen updates and clearing to main loop instead of functions
added a reverse sprite function mainly for trumpet face

v3_4 2022/01/09
added trumpet face sprite
Edited flooring floor sprite as old one looked weird
Adding checking to joy input so x and y screen values can't exceed level size 

v3_3 2022/01/05
Removed joystick issue by setting joyx and joyy to 500 and removing lines setting their base values during setup

v3_2 2022/01/05
Removed mass number from the now named static sprite array to save a few bytes - theyre not affected by gravity
Joystick crashes Arduino in some conditions
Copied in Willy Sprite data from previous versiom

v3_1 2022/01/04
I found the orignal game map for this level so re entered all the coordinates to match
Wrote a demo function to fly through the level for video purposes

v3_0 2022/01/3
All static objects added both sprite bitmaps and locations


*/

//define commands for matrixes and module + screen constants
#include <SPI.h>
//#include <noDelay.h>
// op codes for MAX7219
#define NO_OP 0x00
#define DECODE_MODE 9
#define INTENSITY 10
#define SCAN_LIMIT 11
#define SHUTDOWN 12
#define DISPLAY_TEST 15
//CS pin
#define CSPIN 10
//joystick pins
#define JOYXPIN A0
#define JOYYPIN A1
#define JOYSWITCH 8
//sound
#define BZPIN 6
//module and screen details
#define MODULES2 16
#define MODULES3 24
const byte ROWWIDTH = 8;
const byte COLHEIGHT = 8;
const byte SCREENWIDTH = 32;
const byte SCREENHEIGHT = 24;
const byte LEVELWIDTH = 240;
const byte LEVELHEIGHT = 144;
const uint8_t numOfDevices = 12; //amount of modules all together
byte screenMemory[numOfDevices][8]{0};
byte xScreenStart = 0;
byte yScreenStart = 0;
byte yScreenBase = 0; //used for gravity effect
// this allows me to select which module to display objects on
byte moduleLookupMatrix[4][5]{
	{32, 12, 13, 14, 15},
	{28, 8, 9, 10, 11},
	{24, 4, 5, 6, 7},
	{20, 0, 1, 2, 3}
};
uint16_t sendBuffer[numOfDevices]{0};	 //so we can send information to all modules in one stream
// moving sprites
byte willyHead[8]{
	B00000110, B00111110, B01111100, B00110100, B00111110, B00111100, B00011000, B00111100 //head
}; 
byte willyLegStand[8]{
	B01111110, B01101110, B01101110, B01110110, B00111100, B00011000, B00011000, B00011100 //legs standing
};
byte willyLegWalk1[8]{
	B01111110, B01111110, B11110111, B11111011, B00111100, B01110110, B01101110, B01110111 //walk 1
};
byte willyLegWalk2[8]{
	B01111110, B11111111, B11111111, B10111101, B00111100, B01101101, B11000111, B11100110 //walk 2
};
byte willyHeadLeft[8]{
	B01100000, B01111100, B00111110, B00101100, B01111100, B00111100, B00011000, B00111100 //head left
}; 
byte willyLegStandLeft[8]{
	B01111110, B01110110, B01110110, B01101110, B00111100, B00011000, B00011000, B00111000 //legs standing left
};
byte willyLegWalk1Left[8]{
	B01111110, B01111110, B11101111, B10111111, B00111100, B01101110, B01110110, B11101110 //walk 1 left
};
byte willyLegWalk2Left[8]{
	B01111110, B11111111, B11111111, B10111101, B00111100, B10110110, B11100011, B01100111 //walk 2 left
};
	
uint8_t willyInfo[10]{ 
//x, y, current leg frame, advance frame incremment, mass, direction he's facing left 1/right 0, base for gravity, jumping on off, jumpcounter, jumpheight
	0, 16, 0, 1, 2, 0, 16, 0, 0, 10
};
byte isWillyDead = 0;
byte trumpetFaceTl[8]{
		B00001111, B00011111, B00111100, B00011100, B10001111, B01011111, B11111111, B01011111
}; //top left
byte trumpetFaceBl[8]{
		B10011111, B00001111, B00001111, B10011100, B01111000, B00110000, B00010000, B00001000
}; //bottom left
byte trumpetFaceTr[8]{
		B00001000, B10011000, B11111000, B11111000, B10011000, B11101000, B11110000, B10000000
}; //top right
byte trumpetFaceBr[8]{
		B11110000, B11100000, B11100000, B11100010, B01110100, B00111000, B00010000, B00100000 //bottom right
};
byte trumpetFace2Tl[8]{
		B00001111, B00011111, B00111100, B00011100, B00001111, B00011111, B11111111, B00011111
}; //top left
byte trumpetFace2Bl[8]{
		B00011111, B00001111, B00001111, B00000011, B00000011, B00000011, B00000011, B00000111
}; //bottom left
byte trumpetFace2Tr[8]{
		B00010000, B10110000, B11110000, B11110000, B10110000, B10010000, B11100000, B11110000
}; //top right
byte trumpetFace2Br[8]{
		B11110000, B11100000, B11100000, B00000000, B00000000, B00000000, B00000000, B11000000 //bottom  right
};
byte trumpetFaceInfo[7]{ //x, y, xstart, xend, frame to display, x increment, sprite offset
	56, 64, 56, 112, 0, 1, 8
};
// static sprite info
byte ledge[8]{ //0 in switch
	B11111111, B11111111, B11111111, B11101101, B01110111, B11000010, B01000000, B01000000
};
byte brick[8]{ //1 in switch
	B11100111, B11100111, B00000000, B00111100, B00111100, B00000000, B00000000, B11100111
};
byte bush[8]{ //2 in switch
	B00000011, B10000100, B01011000, B00111010, B00011011, B01011100, B00111000, B00011000
};
byte key[8]{ //3 in switch
	B00110000, B01001000, B10001000, B10010000, B01101000, B00000100, B00001010, B00000100
};
byte spike[8]{ //4 in switch
	B11111111, B11111111, B00111110, B00100100, B00100110, B00000110, B00000100, B00000100
};
byte fallingFloor[8]{ //5 in switch
	B11111111, B11011110, B11001100, B10000000, B00000100, B00000110, B00000001, B00000100
};
byte gate[8]{ //6 in switch
	B11111111, B10011001, B10111011, B11111111, B11111111, B10011001, B10111011, B11111111
};
byte belt[8]{ //7 in switch
	B00001111, B00100000, B01000000, B00000010, B00000100, B00011000, B00111100, B11110000
};
byte keyCounter = 0;
struct dropFloor {
	byte x;
	byte y;
	byte sprite[8];
	byte counter;
	byte yCounter;
};
dropFloor dropFloorArray[13]; //array of structures
//locations according to level map array. Y then x
byte dropFloorLocations[(sizeof(dropFloorArray)/(sizeof(dropFloorArray[0])))][2]{
	// lower floors
	{15, 22},{15, 23},{15, 24},{15, 25},{15, 26},
	//higher floors
	{8, 14},	{8, 15},	{8, 16},	{8, 17},{8, 19},	{8, 20},	{8, 21},	{8, 22},
};
// reminder - [y][x], if anything extra here, add 8 to levelwidth/levelheight variables
byte levelMap[18][30]{
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{8, 8, 8, 8, 8, 8, 8, 8, 3, 8, 4, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 3, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2, 3, 8, 8, 2, 8, 8, 8},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 3},
	{0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 1, 8, 2, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{0, 0, 0, 0, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 2, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 1, 5, 5, 5, 5, 5, 0, 0, 0},
	{8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 6},
	{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 6}
};
byte screenMapLocationX = 0;
byte screenMapLocationY = 0;
//joystick stuff
uint16_t joyX = 500;
uint16_t joyY = 500;
byte joyOffset = 50;
// intro
byte letterM[8] {
	B00000000, B00000000, B00000000, B10001000, B11011000, B10101000, B10001000, B10001000
};
byte letterA[8]{
	B00000000, B00000000, B00000000, B01100000, B10010000, B11110000, B10010000, B10010000
};
byte letterN[8]{
	B00000000, B00000000, B00000000, B10001000, B11001000, B10101000, B10011000, B10001000
};
byte letterI[8]{
	B00000000, B00000000, B00000000, B11110000, B01100000, B01100000, B01100000, B11110000
};
byte letterC[8]{
	B00000000, B00000000, B00000000, B01110000, B10000000, B10000000, B10000000, B01110000
};
byte letterE[8]{
	B00000000, B00000000, B00000000, B11100000, B10000000, B11100000, B10000000, B11100000
};
byte letterR[8]{
	B00000000, B00000000, B00000000, B11100000, B10010000, B11100000, B10100000, B10010000
};
void setup() {
	pinMode(CSPIN, OUTPUT);
	pinMode(JOYXPIN, INPUT);
	pinMode(JOYYPIN, INPUT);
	pinMode(JOYSWITCH, INPUT_PULLUP);
	Serial.begin(9600);
	SPI.begin();
	updateAll(INTENSITY, 0);
	updateAll(DISPLAY_TEST, 0);
	updateAll(DECODE_MODE, 0);
	updateAll(SCAN_LIMIT, 7);
	updateAll(SHUTDOWN, 1);
	pinMode(BZPIN, OUTPUT);
	populateDropFloorArray();
	sendScreenMemory();
	intro();
}
void loop() {
	playGame();
	finishGame();
}
void intro(){
	for (byte i = 0; i < 5; i++)
		{
		plotObject(3, 20, letterM);
		plotObject(9, 18, letterA);
		plotObject(14, 20, letterN);
		plotObject(20, 16, letterI);
		plotObject(25, 18, letterC);
		sendScreenMemory();
		delay(500);
		clearScreenMemory();
		plotObject(3, 16, letterM);
		plotObject(9, 20, letterI);
		plotObject(14, 18, letterN);
		plotObject(20, 20, letterE);
		plotObject(25, 18, letterR);
		sendScreenMemory();
		delay(500);
		clearScreenMemory();
	}	
}
void populateDropFloorArray(){
//used to reset everything
	dropFloorArray[0] = {0, 0, {B11111111, B11011110, B11001100, B10000000, B00000000, B00000000, B00000000, B00000000}, 0, 0 };
	for (byte i = 1; i < (sizeof(dropFloorArray)/(sizeof(dropFloorArray[0]))); i++){
		dropFloorArray[i] = dropFloorArray[0];
	}
	for (byte i = 0; i < (sizeof(dropFloorArray)/(sizeof(dropFloorArray[0]))); i++){
		dropFloorArray[i].y = dropFloorLocations[i][0];		
		dropFloorArray[i].x = dropFloorLocations[i][1];
		levelMap[dropFloorArray[i].y][dropFloorArray[i].x] = 5;
		dropFloorArray[i].counter = 0;
		dropFloorArray[i].yCounter = 0;
	}
}
void finishGame(){
// if willy stands on a key it counts it twice so >= number of keys
	if (keyCounter >= 5){
		invertSprite(gate);
		byte tempWillyY = map(willyInfo[1] + (COLHEIGHT - 1), 0, LEVELHEIGHT, (sizeof(levelMap)/sizeof(levelMap[0])), 0); //top of his frame
		byte tempWillyX = map(willyInfo[0], 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0])));
		if (levelMap[tempWillyY][tempWillyX] == 6){
			for (byte j = 0; j < 255; j++){
				tone(BZPIN, 1520 - j, 20);
				delay(10);
			}
			isWillyDead = 1;
		}
	}
}
void playGame(){
	checkJoyInput();
	screenMapLocationX = map(xScreenStart, 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0])));
	screenMapLocationY = map(yScreenStart + COLHEIGHT, 0, LEVELHEIGHT, ((sizeof(levelMap)/sizeof(levelMap[0])) - 1), 0); //+colheight as starts from top left of module. -1 because of this
	displayLevel();
	showAnimatedObjects();
	sendScreenMemory(); // added extra send here as it looks a bit better even with flickers		
	showWilly();
	sendScreenMemory();	
	clearScreenMemory();
	willyDeadCheck();
	delay(80);
	noTone(BZPIN);
}
void willyDeadCheck(){
	if (isWillyDead == 1){
		//flash then reset everything
		updateAll(INTENSITY, 3);
		for (byte i = 0; i < 8; i++){
			tone(BZPIN, 520 * i, 20);
			delay(10);
		}
		populateDropFloorArray();
		delay(40);
		xScreenStart = 0;
		yScreenStart = 0;
		willyInfo[0] = 0;
		willyInfo[1] = 16;
		willyInfo[7] = 0;
		willyInfo[8] = 0;
//		keyCounter = 0;
		updateAll(INTENSITY, 0);
		isWillyDead = 0;
	}
}
void reverseSprite(byte array[]){
//MSB of sprite array to LSB of new array
	byte tempArray[8];
		for (byte i = 0; i < ROWWIDTH; i++){
					for (byte t = 0; t < COLHEIGHT; t++){	
				bitWrite(tempArray[t], i, (bitRead(array[t], (7 - i))));	
			}	
		}
	for (byte i = 0; i < COLHEIGHT; i++){
		array[i] = tempArray[i];
	}
}
void invertSprite(byte array[]){
	for (byte i = 0; i < COLHEIGHT; i++){
		array[i] = ~array[i];
	}
}
void showAnimatedObjects(){
//i hate this section but Ive coded this to only with with 8 bit arrays and dont know how to pass 2d arrays without breaking it
// all to display trumpetface relative to one set of x, y coordinates
	if (trumpetFaceInfo[4] == 0){
		plotObject(trumpetFaceInfo[0], trumpetFaceInfo[1], trumpetFaceTl);
		plotObject(trumpetFaceInfo[0], trumpetFaceInfo[1] - COLHEIGHT, trumpetFaceBl);
		plotObject(trumpetFaceInfo[0] + trumpetFaceInfo[6], trumpetFaceInfo[1], trumpetFaceTr);
		plotObject(trumpetFaceInfo[0] + trumpetFaceInfo[6], trumpetFaceInfo[1] - COLHEIGHT, trumpetFaceBr);
		trumpetFaceInfo[4] = 1;
	}
	else {
		plotObject(trumpetFaceInfo[0], trumpetFaceInfo[1], trumpetFace2Tl);
		plotObject(trumpetFaceInfo[0], trumpetFaceInfo[1] - COLHEIGHT, trumpetFace2Bl);
		plotObject(trumpetFaceInfo[0] + trumpetFaceInfo[6], trumpetFaceInfo[1], trumpetFace2Tr);
		plotObject(trumpetFaceInfo[0] + trumpetFaceInfo[6], trumpetFaceInfo[1] - COLHEIGHT, trumpetFace2Br);
		trumpetFaceInfo[4] = 0;		
	}
	trumpetFaceInfo[0] = trumpetFaceInfo[0] + trumpetFaceInfo[5];
//if trumpet face at start or end of his roaaming range, reverse the sprite
	if ((trumpetFaceInfo[0] >= trumpetFaceInfo[3]) | (trumpetFaceInfo[0] <= trumpetFaceInfo[2])){
		trumpetFaceInfo[5] = -trumpetFaceInfo[5];
		trumpetFaceInfo[6] = -trumpetFaceInfo[6];
		reverseSprite(trumpetFaceTl);
		reverseSprite(trumpetFaceBl);
		reverseSprite(trumpetFaceTr);
		reverseSprite(trumpetFaceBr);
		reverseSprite(trumpetFace2Tl);
		reverseSprite(trumpetFace2Bl);
		reverseSprite(trumpetFace2Tr);
		reverseSprite(trumpetFace2Br);		
	}
	reverseSprite(belt);
	trumpetFaceCollisionDetect();
}
void trumpetFaceCollisionDetect(){
	if ((willyInfo[1] < trumpetFaceInfo[1] - 4) & (willyInfo[1] > (trumpetFaceInfo[1] - MODULES2) + 1) | ((willyInfo[1] - MODULES2) + 1 < trumpetFaceInfo[1] - 4) & ((willyInfo[1] - MODULES2) + 1 > (trumpetFaceInfo[1] - MODULES2) + 1)){
		if ((willyInfo[0] > trumpetFaceInfo[0]) & (willyInfo[0] < (trumpetFaceInfo[0] + MODULES2) - 4) | ((willyInfo[0] + ROWWIDTH) - 1 > trumpetFaceInfo[0]) & ((willyInfo[0] + ROWWIDTH) - 1 < (trumpetFaceInfo[0] + MODULES2) - 4)){
			isWillyDead = 1;
		}
	}
}
void showWilly(){
// check which way he's facing and plot appropiately
	if (willyInfo[5] == 0){
		plotObject(willyInfo[0], willyInfo[1], willyHead);
	}
	else {
		plotObject(willyInfo[0], willyInfo[1], willyHeadLeft);
	}
	plotObject(willyInfo[0], willyInfo[1] - COLHEIGHT, checkWillyLegs(willyInfo[2]));
	willyJumping();
}
void willyMovements(){
//increment or decrement what needs to be
//move the screen relative to the direcion willy is facing
//moves to right of screen if facing left
	if (willyInfo[5] == 0 & (willyInfo[0] < (xScreenStart + MODULES3))){ //not sure why I put 2nd conidition but everything working
		willyInfo[0] ++; //add to x
		willyInfo[2] += willyInfo[3]; //increase leg frame by frane increment 
		if (willyInfo[2] == 0 || willyInfo[2] == 2){
			willyInfo[3] = -willyInfo[3]; //cycle through leg frames
		}
		if (willyInfo[0] > SCREENWIDTH & (willyInfo[0] + SCREENWIDTH) < LEVELWIDTH){
			xScreenStart = willyInfo[0];
		}
	}
	if (willyInfo[5] == 1 & (willyInfo[0] > xScreenStart)){
		willyInfo[0] --; //MINUS from x
		willyInfo[2] += willyInfo[3]; //increase leg frame by frane increment 
		if (willyInfo[2] == 0 || willyInfo[2] == 2){
			willyInfo[3] = -willyInfo[3];
		}
		if (willyInfo[0] > SCREENWIDTH & (xScreenStart + MODULES3) < LEVELWIDTH){ //move willy to right of screen
			xScreenStart = willyInfo[0] - MODULES3;
		}
	}
}
void willyJumping(){
	if (willyInfo[7] == 1){
		if (willyInfo[8] < willyInfo[9]){
			willyInfo[8] ++;
			willyInfo[1] += willyInfo[4];
			yScreenStart += willyInfo[4];
			willyCollisionDetectHead();
			tone(BZPIN, (240 * willyInfo[8]), 20);
			}
		else {
			willyInfo[7] = 0;
			willyInfo[8] = 0;
		}
	}
	else if (willyCollisionDetectFeet() == 0 & willyInfo[7] == 0) {
		tone(BZPIN, (240 - (willyInfo[1] * 10)), 10);
		gravity(willyInfo[1], 1, willyInfo[6]);
		gravity(yScreenStart, 1, 0);
	}
}
byte willyCollisionDetectLAndR(){
	byte tempWillyY = map((willyInfo[1] - (COLHEIGHT - 1)), 0, LEVELHEIGHT, (sizeof(levelMap)/sizeof(levelMap[0])), 0); // bottom of his frame
	byte tempWillyX = map((willyInfo[0]), 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0]))); //left of his frame
	byte tempWillyX2 = map((willyInfo[0] + (ROWWIDTH - 1)), 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0]))); //right of his frame
	if (tempWillyY < (sizeof(levelMap)/sizeof(levelMap[0]))){ //this is so gravity works and stops
		if (levelMap[tempWillyY][tempWillyX] != 8 | levelMap[tempWillyY][tempWillyX2] != 8){
				checkWhatWillyTouchesLAndR(levelMap[tempWillyY][tempWillyX]);		
				checkWhatWillyTouchesLAndR(levelMap[tempWillyY][tempWillyX2]);
		}
	}
}
byte checkWhatWillyTouchesLAndR(byte &object){
		if (object == 1){	
   		if (willyInfo[5] == 0){
			   xScreenStart --;
			   willyInfo[0] --;
			}
		   else {
			   xScreenStart ++;
			   willyInfo[0] ++;
			}	
		}
		else if (object == 2){
			isWillyDead = 1;			
		}
		else if (object == 4){
			isWillyDead = 1;
		}
	   else if (object == 3){
		   keyCounter ++;
			object = 8;
		}
}
byte willyCollisionDetectHead(){
	byte tempWillyY = map(willyInfo[1] + (COLHEIGHT - 1), 0, LEVELHEIGHT, (sizeof(levelMap)/sizeof(levelMap[0])), 0); //top of his frame
	byte tempWillyX = map(willyInfo[0], 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0]))); //left of his frame
	byte tempWillyX2 = map((willyInfo[0] + (ROWWIDTH - 1)), 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0]))); //right of his frame
	checkWhatWillyTouches(levelMap[tempWillyY][tempWillyX], 0);
	checkWhatWillyTouches(levelMap[tempWillyY][tempWillyX2], 0);
}
byte willyCollisionDetectFeet(){
//checking whats under willy's feet
	byte tempWillyY = map((willyInfo[1] - (COLHEIGHT + 1)), 0, LEVELHEIGHT, (sizeof(levelMap)/sizeof(levelMap[0])), 0); // bottom of his frame
	byte tempWillyX = map(willyInfo[0], 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0]))); //left of his frame
	byte tempWillyX2 = map((willyInfo[0] + (ROWWIDTH - 1)), 0, LEVELWIDTH, 0, (sizeof(levelMap[0])/sizeof(levelMap[0][0]))); //right of his frame
	if (tempWillyY < (sizeof(levelMap)/sizeof(levelMap[0]))){
		if (levelMap[tempWillyY][tempWillyX] != 8 | levelMap[tempWillyY][tempWillyX2] != 8){
			if (levelMap[tempWillyY][tempWillyX] != 5 | levelMap[tempWillyY][tempWillyX2] != 5){
				checkWhatWillyTouches(levelMap[tempWillyY][tempWillyX], 1);
				checkWhatWillyTouches(levelMap[tempWillyY][tempWillyX2], 1);
				return 1;
			}
			if (levelMap[tempWillyY][tempWillyX] == 5 | levelMap[tempWillyY][tempWillyX2] == 5) {
				for (byte g = 0; g < (sizeof(dropFloorArray)/(sizeof(dropFloorArray[0]))); g++){
					if (dropFloorArray[g].y == tempWillyY){
						if (dropFloorArray[g].x == tempWillyX2){
							dropFloorArray[g].counter ++;
							//below handles dropping the floor
							if (dropFloorArray[g].counter >= 8){
								dropFloorArray[g].counter = 0;
								dropFloorArray[g].sprite[(0 + dropFloorArray[g].yCounter)] = 0;
								dropFloorArray[g].yCounter ++;
								gravity(willyInfo[1], 1, willyInfo[6]);
								gravity(yScreenStart, 1, 0);
							}
							//remove floor from level after a certain point
							if (dropFloorArray[g].yCounter == 4){
								levelMap[dropFloorArray[g].y][dropFloorArray[g].x] = 8;
								dropFloorArray[g].yCounter = 0;
							}					
						}
					}
				}
				return 1;
			}
		}
		else {
			return 0;
		}
	}
}
void checkWhatWillyTouches(byte &object, byte feet){
// combined collision detect for head and feet. Head = 0
	if (feet = 1){			
		if (object == 1){
			willyInfo[7] = 0;
			willyInfo[8] = 0;
		}
		else if (object == 2){
			isWillyDead = 1;			
		}
		else if (object == 4){
			isWillyDead = 1;
		}
		else if (object == 7){
			willyInfo[0] --;
			xScreenStart --;
			willyInfo[5] = 1;	
			willyCollisionDetectLAndR();
			trumpetFaceCollisionDetect(); //extra otherwise he doesnt detect objects on conveyor belt
		}
		else if (object == 3){
			keyCounter ++;
			object = 8;
		}
	}
	if (feet = 0){	
		if (object == 1){
			willyInfo[7] = 0;
			willyInfo[8] = 0;
		}
		else if (object == 2){
			isWillyDead = 1;			
		}
		else if (object == 4){
			isWillyDead = 1;
		}
		else if (object == 7){
			willyInfo[7] = 0;
			willyInfo[8] = 0;
		}
		else if (object == 3){
			keyCounter ++;
			object = 8;
		}
	
	}
}
byte* checkWillyLegs(uint8_t legs){
//returns pointer to correct leg sprite
	if (willyInfo[5] == 0){
		switch(legs){
			case 0:
			return willyLegStand;
			case 1: 
			return willyLegWalk1;
			case 2:
			return willyLegWalk2;
			}
	}
	else {
		switch(legs){
			case 0:
			return willyLegStandLeft;
			case 1: 
			return willyLegWalk1Left;
			case 2:
			return willyLegWalk2Left;
		}
	}
}
byte* checkObject(uint8_t object){
//returns a pointer to an array
	switch(object){
		case 0:
		return ledge;
		case 1:
		return brick;
		case 2:
		return bush;
		case 3:
		return key;
		case 4:
		return spike;
		case 5:
		break;
//		return fallingFloor;
		case 6:
		return gate;
		case 7:
		return belt;
	}
}
byte gravity(byte &y, byte spriteMass, byte stopAt){
	if (y > stopAt){
		y = y - spriteMass;
	}
	return y;
}
void displayLevel(){
// -j as the y axis is larger number at the bottom and reduces up the screen
	for (byte j = 0; j < 4; j++){
		for (byte i = 0; i < 5; i++){
			if (!((levelMap[screenMapLocationY - j][(screenMapLocationX + i)]) == 8)){
				if(!((levelMap[screenMapLocationY - j][(screenMapLocationX + i)]) == 5)){
					plotObject(((screenMapLocationX + i) * ROWWIDTH), (((sizeof(levelMap)/sizeof(levelMap[0])) - (screenMapLocationY - j)) * COLHEIGHT), (checkObject(levelMap[screenMapLocationY - j][screenMapLocationX + i])));
				}
				else if (levelMap[screenMapLocationY - j][(screenMapLocationX + i)] == 5) {
					for (byte g = 0; g < (sizeof(dropFloorArray)/(sizeof(dropFloorArray[0]))); g++){
						if (dropFloorArray[g].y == screenMapLocationY - j){
							if (dropFloorArray[g].x == screenMapLocationX + i){
							plotObject(((screenMapLocationX + i) * ROWWIDTH), (((sizeof(levelMap)/sizeof(levelMap[0])) - (screenMapLocationY - j)) * COLHEIGHT) - dropFloorArray[g].yCounter, dropFloorArray[g].sprite);	
							}
						}
					}
				}
			}
		}
	}
}
void checkJoyInput(){
	uint16_t x = analogRead(JOYXPIN);
	uint16_t y = analogRead(JOYYPIN);
	byte switchState = digitalRead(JOYSWITCH);
	if ( x < (joyX - joyOffset)){
		if (xScreenStart != 0){
			xScreenStart --;
		}
		willyInfo[5] = 1;
		willyMovements();
		willyCollisionDetectLAndR();
	}
	if (x > (joyX + joyOffset)){
		if ((xScreenStart + SCREENWIDTH) < LEVELWIDTH){
			xScreenStart ++;
		}
		willyInfo[5] = 0;
		willyMovements();
		willyCollisionDetectLAndR();
	}
	if ( y < (joyY - joyOffset)){
		if ((yScreenStart + SCREENHEIGHT) < LEVELHEIGHT){
			yScreenStart ++;
		}
	}
	if (y > (joyY + joyOffset)){
		if (yScreenStart != 0){
			yScreenStart --;
		}
	}
	if (switchState == 0 & willyInfo[7] == 0){
		willyInfo[7] = 1;
	}
}
void checkKeyboardInput(){
	uint8_t input = Serial.read();
	switch(input){
		case 113 : //q is for moving screen up
		   yScreenStart += 4;
			break;
		case 97 : // a is for moving screen down
		   if (yScreenStart != 0){
		      yScreenStart -= 4;
			}
		   break;
		case 112: //112 = p for moving right
		   xScreenStart += 4;
			break;
		case 111: //111 = lower case o for moving left
		   if (xScreenStart != 0){
			   xScreenStart -= 4;
		   } 
			break;
		case 114: // 114 = lower case r resets display and screenMemory
//		   clearAllDisplays();
		   clearScreenMemory();
		sendScreenMemory();
			break;
		case 100: // 100 = d jump
//		if (willyInfo[7] = 0){
		willyInfo[7] = 1;			
//		}

		   break;
	}
}
void displayPlottedObject(byte startModule, byte x, byte y, byte colCounter, byte rowCounter, byte array[]){
//display what will fit on correct module
		byte i = 0;
		for (byte t = (colCounter-y); t < COLHEIGHT; t++){
		    if (startModule < numOfDevices | startModule >= 20){
			    byte s = 0;
		//the below loop - starts at x point (r) but need to start from MSB copying image array
		       for (byte r = (x - rowCounter); r < ROWWIDTH; r++){
				        bitWrite(screenMemory[startModule][t], (7 - r), (bitRead(array[i], (7 - s))));
			    s++;
			    }
		//put x reminder on next module
			    if (s < ROWWIDTH){
				 byte m =0;
				  for (s; s < ROWWIDTH; s++){
					if (startModule >= 20){
						bitWrite(screenMemory[startModule - 20][t], (7 - m), (bitRead(array[i], (7 - s))));
					}	
					else if (!(startModule == 3 || startModule == 7 || startModule == 11)){
					     bitWrite(screenMemory[startModule+1][t], (7 - m), (bitRead(array[i], (7 - s))));
					   }
					m++;
				  }
			   }
			}
		i++;
		}
//display y remainder on next module
		if (i < COLHEIGHT && startModule > 3){
			byte g = 0;
			for (i; i < COLHEIGHT; i++){
				byte	s = 0;
		/*the below loop - starts at x point (r) but need to start from MSB copying image array*/
		      for (byte r = (x - rowCounter); r < ROWWIDTH; r++){
					bitWrite(screenMemory[startModule-4][g], (7 - r), (bitRead(array[i], (7 - s))));
					s++;
				}
			// put x remainder on next module
			if (s < ROWWIDTH){
				byte m = 0;
				for (s; s < ROWWIDTH; s++){
					if (startModule >= 20){
						bitWrite(screenMemory[(startModule - 24)][g], (7 - m), (bitRead(array[i], (7 - s))));
					}
					else if (!(startModule == 3 || startModule == 7 || startModule == 11)){
						bitWrite(screenMemory[(startModule - 4) +1][g], (7 - m), (bitRead(array[i], (7 - s))));
					}
				   m++;
				}
			}	
		g++;
		}	
	}
}	
	
void plotObject(byte x, byte y, byte array[]){
	byte moduleX = 0;
	byte moduleY = 0;
	byte colCounter = 0;
	byte rowCounter = 0;
//uses x and y values against a lookup array to determine which module to start on
	if (x >= (xScreenStart - ROWWIDTH) && x < xScreenStart){
		moduleX = 0;
		rowCounter = (xScreenStart - ROWWIDTH);
	}
	else if (x >= xScreenStart && x < (xScreenStart + ROWWIDTH)){
		moduleX = 1;
		rowCounter = xScreenStart;
	}
	else if (x >= (xScreenStart + ROWWIDTH) && x < (xScreenStart + MODULES2)){
		moduleX = 2;
		rowCounter = (xScreenStart + ROWWIDTH);
	}
	else if (x >= (xScreenStart + MODULES2) && x < (xScreenStart + MODULES3)){
		moduleX = 3;
		rowCounter = (xScreenStart + MODULES2);
	}
	else if (x >= (xScreenStart + MODULES3) && x < (xScreenStart + SCREENWIDTH)){
		moduleX = 4;
		rowCounter = (xScreenStart + MODULES3);
	}
	else {
		moduleX = 10;
	}
	if (y < yScreenStart){
		moduleY = 10;
	}	
	else if (y >= yScreenStart && y < (yScreenStart + COLHEIGHT)){
		moduleY = 3;
		colCounter = (yScreenStart + COLHEIGHT);
	}
	else if (y >= (yScreenStart + COLHEIGHT) && y < (yScreenStart + MODULES2)){
		moduleY = 2;
		colCounter = (yScreenStart + MODULES2);
	}
	else if (y >= (yScreenStart + MODULES2) && y < (yScreenStart + SCREENHEIGHT)){
		moduleY = 1;
		colCounter = (yScreenStart + SCREENHEIGHT); //changed from = screenheight only
	}
	else if (y >= (yScreenStart + SCREENHEIGHT) && y < (yScreenStart + SCREENHEIGHT + COLHEIGHT)){
		moduleY = 0;
		colCounter = (yScreenStart + SCREENHEIGHT + COLHEIGHT);
		}
	else {
		moduleY = 10;
	}
	//added the below so not to waste trying to plot something that wont show
	if (!(moduleX == 10 || moduleY == 10)){
		displayPlottedObject(moduleLookupMatrix[moduleY][moduleX], x, y, colCounter, rowCounter, array);
	}		
}
void rotateModules90(){
//rotates all modules anti-clockwise
	for (byte g = 0; g < numOfDevices; g++){
		byte tempArray[8]{0};
		byte k = 7;
		for (byte i = 0; i < COLHEIGHT; i++){
			for (byte j = 0; j < ROWWIDTH; j++){
				bitWrite(tempArray[j], k, (bitRead(screenMemory[g][i], j)));
			}
			k--;
		}
		for (byte f = 0; f < COLHEIGHT; f++){
			screenMemory[g][f] = tempArray[f];
		}
	}
sendScreenMemory();
}
void sendScreenMemory(){
	for (byte j = 0; j < COLHEIGHT; j++){
		for (byte i = 0; i < numOfDevices; i++){
			sendBuffer[i] = (j + 1) << 8 | screenMemory[i][j];
		}
		SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
		digitalWrite(CSPIN, LOW);
		for (byte k = 0; k < numOfDevices; k++){
			SPI.transfer16(sendBuffer[k]);
		}
		digitalWrite(CSPIN, HIGH);
		SPI.endTransaction();	
	}
}
void showObject(uint8_t startModule, byte array[8]){
		for (byte t = 0; t < COLHEIGHT; t++){
			screenMemory[startModule][t] = array[t];
		}
	sendScreenMemory();
}
void updateAll(uint16_t cmd, uint8_t data){
//used for sending operation codes
	uint16_t x = (cmd << 8) | data;
	SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
	digitalWrite(CSPIN, LOW);
	for (byte i = 0; i < numOfDevices; i++){
	SPI.transfer16(x);
		}
	digitalWrite(CSPIN, HIGH);
	SPI.endTransaction();
}
void clearAllDisplays(){
//turns all LEDs on all displays off
	for (byte i = 0; i < numOfDevices; i++){
		for (byte j = 1; j <= COLHEIGHT; j++){ //1 to 8 as a 0 means no op
			SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
			digitalWrite(CSPIN, LOW);			
			uint16_t x = (j << 8) | B00000000;
			SPI.transfer16(x);
			digitalWrite(CSPIN, HIGH);
			SPI.endTransaction();
		}
	}
}
void clearScreenMemory(){
//wipe the screen memory array
	for (byte i = 0; i < numOfDevices; i++){
		for (byte j = 0; j < ROWWIDTH; j++){
			screenMemory[i][j] = 0;
		}
	}
}
void clearSendBuffer(){
//empty all data from send buffer
 for (byte i = 0; i < numOfDevices; i++){
		sendBuffer[i] = 0;
	}
}