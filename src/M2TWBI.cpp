/**
  Simple Keyboard hook
  Limitations: 
    Min Version: Windows 98
    Handles Window Titles up to 255 characters long
    Support for [A-Za-z0-9 {TAB}{DELETE}{BACKSPACE}{RETRUN}] CTRL(Async), SHIFT(Async)
    Max buffer = 512 -> TODO:dynamic buffer size,
      -> realloc attempts have prepended some garbage values to buffer
*/

//Skeleton Includes
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
//Usage includes
#include <ctype.h>
#include <string>

using namespace std;
//WinAPI includes
#define WINVER 0x500 //win98
#include <Windows.h>
#include <tchar.h>





  DWORD pid;
HANDLE phandle;

//All the Addresses:
	
	unsigned long charBaseAd0 = 0x199e188;
	unsigned long charBaseAd1;
	unsigned long charBaseAd2;
	unsigned long charBaseAd3;
	unsigned long genCamBaseAd0 = 0x195dcf0;
	unsigned long genCamBaseAd1;
	unsigned long charOrderBaseAd = 0x199e188;
	unsigned long genCamTriggerAd = 0x195dcf8;
	unsigned long gameSpeedAd = 0x1b8414c;
	unsigned long cameraXAd = 0x195f93c;
	unsigned long cameraYAd = 0x195f944;
	unsigned long cameraZAd = 0x195f95c;
	unsigned long cameraRAd = 0x195f958;
	unsigned long cursorXAd = 0x16d3c84;
	unsigned long screenWidthAd = 0x162fb84;
	unsigned long charFuck0;
	unsigned long charFuck1;
	unsigned long charFuck2;
	
	
// All the control offsets:
	
	unsigned long charXOff = 0x128;
	unsigned long charYOff = 0x130;
	unsigned long charROff = 0x134;
	unsigned long charMovOff0 = 0x17c;
	unsigned long charMovOff1 = 0x148;
	unsigned long charRealMovOff = 0x140;
	unsigned long charTurnOff = 0x19c;
	unsigned long charMyst0 = 0x214;
	unsigned long charMyst1 = 0x21c;
	unsigned long charAnimOff = 0x24c;
	unsigned long charSpeedOff = 0x120;
	unsigned long charStopFuckingX = 0x154;
	unsigned long charStopFuckingY = 0x158;
	
	unsigned long genCamXOff = 0xc;
	unsigned long genCamYOff = 0x10;
	unsigned long genCamROff = 0x14;
	unsigned long genCamHOff = 0x20;


// All the key controllers
	
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
	bool q = false;
	bool e = false;
	bool shift = false;



// All the variables

	bool active = false;
	int genCam = 1;
	unsigned long charBase1Old = 0x0;
	int perfCounter = 0;
	bool mousePause= false;
	int attack = 18;
	bool turning = false;
	
	
	float charX;
	float charY;
	int charR;
	float camX = 3.0f;
	float camY = 1.7f;
	float slowmotion = 0.2f;
	float normal = 1.0f;
	float camSwitchZ = 4.0f;
	int camH = 3191899074;
	int camR;
	int screenWidth;
	int standing = 1;
	int walking = 2;
	int running = 3;
	int realMove = 0;
	float left = -0.8f;
	float right = 0.8f;
	float ahead = 0.0f;
	int myst0 = 11;
	int myst1 = 10;
	int leftT = 5;
	int rightT = 2;
	int rightS = 9;
	int leftS = 8;
	int backwards = 7;
	int zero = 0;
	int cursorX;
	int cursorXOld;







//processKB :: int, WPARAM, LPARAM -> CALLBACK
//Deals with processing MSG from keyboard.
LRESULT CALLBACK processKB( int code, WPARAM event, LPARAM kb );

//parseChar :: int, bool[=true] -> void
//Determines what to send to output for a given virtural key.
void parseChar(int c, bool down=true);
void readwriteMemory();
void baseSetup();
void moveCamToPlayer();
void mousemove();
void forceGenCam();
//Global Variables, No good method for passing in this program
char* buffer = (char*)malloc(sizeof(char)*512); //current window buffer

string buf = ""; //Complete buffer
HWND lastProc  =NULL;
//chr2str :: char -> char*
//converts single character to char*(string).



//output :: char* -> void
//Handles output for program of strings only.


int main(){
  buffer[0] = '\0';
  lastProc  = FindWindow(NULL,"Medieval 2");

  HINSTANCE appInstance = GetModuleHandle(NULL);//App Instance for call back

  SetWindowsHookEx( WH_KEYBOARD_LL, processKB, appInstance, 0 );
  
  

	
	GetWindowThreadProcessId(lastProc,&pid);
	phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE,pid);
	
	//Sleep(10000);
	ReadProcessMemory(phandle, (void*)(screenWidthAd),&screenWidth,sizeof(int),0);
	printf("%d",screenWidth);
	screenWidth = screenWidth/2;
	
while(true){

  while(active == false){
  	
  	Sleep(500);
  	baseSetup();
  	
}
  
  
  printf("\nrunning!");
  printf("\n%d",charBaseAd0);
  
printf("\n%d",charOrderBaseAd);
  while(active == true){
  readwriteMemory();
  //Sleep(1);


  MSG msg; //message recieved
  while(PeekMessage(&msg, NULL, 0, 0,PM_REMOVE) > 0) {

    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
 

}
}
  return EXIT_SUCCESS;

}

//parseChar :: int, bool[=true] -> void
//Determines what to send to output for a given virtural key.
void parseChar(int c, bool down) {

  
  int forward = 1;
  if(c==0x55){
  	w = down;
  }
  if(c==0x48){
  	a = down;
  }
  if(c==0x4b){
  	d = down;
  }
  if(c==0x59){
  	q = down;
  }
  if(c==0x49){
  	e = down;
  }
  if(c==0x4a){
  	s = down;
  }
  if(c==0x56){
  	shift = down;
  }
  if(c==0x22){
  	forceGenCam();
  }
  if(c==0x2E&&down == true &&genCam == 2){
  	genCam = 1;
  	moveCamToPlayer();
	  }
	  
  if(c==0x30&&down == false){
  	if(slowmotion == 1.0f){
  		slowmotion = 0.2f;
  	}else{
  		slowmotion = 1.0f;
  	}
  } 
   
}

//processKB :: int, WPARAM, LPARAM -> CALLBACK
//Deals with processing MSG from keyboard.
LRESULT CALLBACK processKB( int code, WPARAM event, LPARAM kb ) {

  KBDLLHOOKSTRUCT* ptrKB = (KBDLLHOOKSTRUCT*)kb;//ptr to keyboard hook(struct)
  switch(event) {

    case WM_KEYUP:
      parseChar(ptrKB->vkCode, false);

    break;
    case WM_KEYDOWN:
      parseChar(ptrKB->vkCode);
      
      

    break;
  }
  return CallNextHookEx(NULL, code, event, kb );
}





void mousemove(){
	
	ReadProcessMemory(phandle, (void*)(cursorXAd),&cursorX,sizeof(int),0);
	ReadProcessMemory(phandle, (void*)(genCamBaseAd1+genCamROff),&camR,sizeof(int),0);
	camR += ((cursorX - cursorXOld)*70);
	if(cursorX != cursorXOld && mousePause ==false){
	WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamROff),&camR,sizeof(int),0);
	}
	else{
		mousePause = false;
	}
	cursorXOld = cursorX;
	if(cursorX < 50){
	SetCursorPos(screenWidth,200);
	cursorXOld = 512;
	mousePause = true;
}else if (cursorX > 954){
	SetCursorPos(screenWidth,200);
	cursorXOld = 512;
	mousePause = true;
}


}

void baseSetup(){
	//Finding the chars base address (Resulting address: charBaseAd1!)
	
	ReadProcessMemory(phandle,(void*)charBaseAd0,&charBaseAd1,sizeof(int),0);
	if(charBaseAd1 != charBase1Old){
		
	ReadProcessMemory(phandle, (void*)(charBaseAd1+0x54c),&charBaseAd2,sizeof(int),0);
	ReadProcessMemory(phandle, (void*)(charBaseAd2+0x4e0),&charBaseAd3,sizeof(int),0);
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charROff),&charR,sizeof(int),0);
	active = true;
	ReadProcessMemory(phandle, (void*)(charBaseAd0),&charOrderBaseAd,sizeof(int),0);
	ReadProcessMemory(phandle, (void*)(charOrderBaseAd+0x280),&charOrderBaseAd,sizeof(int),0);
}
//printf("\nchecked!");
//printf("%d",charOrderBaseAd);
charBase1Old = charBaseAd1;
}

void forceGenCam(){
		WriteProcessMemory(phandle, (void*)(gameSpeedAd),&normal,sizeof(float),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamXOff),&camX,sizeof(float),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamYOff),&camY,sizeof(float),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamROff),&charR,sizeof(int),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamHOff),&camH,sizeof(int),0);
		genCam = 2;
}

void genCamSetup(){
	//Finding the chars base address (Resulting address: charBaseAd3!)
	
	ReadProcessMemory(phandle,(void*)genCamBaseAd0,&genCamBaseAd1,sizeof(int),0);
	ReadProcessMemory(phandle, (void*)(genCamBaseAd1+0x4),&genCamBaseAd1,sizeof(int),0);
	genCam = 2;
		WriteProcessMemory(phandle, (void*)(genCamTriggerAd),&genCam,sizeof(int),0);
		WriteProcessMemory(phandle, (void*)(gameSpeedAd),&normal,sizeof(float),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamXOff),&camX,sizeof(float),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamYOff),&camY,sizeof(float),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamROff),&charR,sizeof(int),0);
		WriteProcessMemory(phandle, (void*)(genCamBaseAd1+genCamHOff),&camH,sizeof(int),0);
}

void moveCamToPlayer(){
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charXOff),&charX,sizeof(float),0);
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charYOff),&charY,sizeof(float),0);
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charROff),&charR,sizeof(float),0);
	WriteProcessMemory(phandle, (void*)(cameraXAd),&charX,sizeof(float),0);
	WriteProcessMemory(phandle, (void*)(cameraYAd),&charY,sizeof(float),0);
	WriteProcessMemory(phandle, (void*)(cameraRAd),&charR,sizeof(float),0);
	WriteProcessMemory(phandle, (void*)(cameraZAd),&camSwitchZ,sizeof(float),0);
}

void readwriteMemory(){
// First of all, the Movement speed

if(w == true){
	myst0 = 11;

		if(shift == false){
			WriteProcessMemory(phandle, (void*)(charBaseAd3+charMovOff0),&running,sizeof(int),0);
			if(realMove ==running)
			WriteProcessMemory(phandle, (void*)(charBaseAd3+charMovOff1),&running,sizeof(int),0);
		}
		else{
			WriteProcessMemory(phandle, (void*)(charBaseAd3+charMovOff0),&walking,sizeof(int),0);
			if(realMove ==walking)
			WriteProcessMemory(phandle, (void*)(charBaseAd3+charMovOff1),&walking,sizeof(int),0);
		}
	}
	else{
		WriteProcessMemory(phandle, (void*)(charBaseAd3+charMovOff0),&standing,sizeof(int),0);
	}
	
if(perfCounter%2 == 1){
// Now the Turning while walking/running
if(a == true/*&&myst0 == 11*/){
	//WriteProcessMemory(phandle, (void*)(charBaseAd3+charTurnOff),&left,sizeof(float),0);
	charR -=1;
}
else if(d==true/*&&myst0 == 11*/){
	//WriteProcessMemory(phandle, (void*)(charBaseAd3+charTurnOff),&right,sizeof(float),0);
	charR +=1;
}
else{
	//WriteProcessMemory(phandle, (void*)(charBaseAd3+charTurnOff),&ahead,sizeof(float),0);
}

}
// The shuffle/turn animation in different directions
turning = false;
if (s==true){
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charAnimOff),&backwards,sizeof(int),0);
}
else if (q==true){
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charAnimOff),&leftS,sizeof(int),0);
}
else if (e==true){
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charAnimOff),&rightS,sizeof(int),0);
}
/*}
else{
 if (d==true){
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charAnimOff),&rightT,sizeof(int),0);
}
else if (a==true){
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charAnimOff),&leftT,sizeof(int),0);
}
else{
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charAnimOff),&zero,sizeof(int),0);
}*/



//mystical numbers here (I have no clue why they have these values, i just know it works)


WriteProcessMemory(phandle, (void*)(charBaseAd3+charMyst0),&myst0,sizeof(int),0);
WriteProcessMemory(phandle, (void*)(charBaseAd3+charMyst1),&myst1,sizeof(int),0);
	WriteProcessMemory(phandle, (void*)(charBaseAd3+charROff),&charR,sizeof(int),0);





//mouse camera movement

if(genCam == 2)
mousemove();


//character speed is set to normal

WriteProcessMemory(phandle, (void*)(charBaseAd3+charSpeedOff),&normal,sizeof(int),0);



perfCounter++;

/*if(perfCounter == 125){
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charXOff),&charX,sizeof(float),0);
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charYOff),&charY,sizeof(float),0);
}

if(perfCounter == 375){
	WriteProcessMemory(phandle, (void*)(charOrderBaseAd+charStopFuckingX),&charX,sizeof(float),0);
	WriteProcessMemory(phandle, (void*)(charOrderBaseAd+charStopFuckingY),&charY,sizeof(float),0);
}*/

if(perfCounter == 250){
	if(active == true){
		//Test if battle is still running
		int test = 100;
		ReadProcessMemory(phandle, (void*)(charBaseAd3+charMovOff0),&test,sizeof(int),0);
		if(test == 100){//if theres no battle information on this address anymore/IF NOT OVERWRITTEN
		active = false;
		printf("\ninactive!");
		}
	}
}


if(perfCounter == 750){

	//Check for a new genCam-Address
	ReadProcessMemory(phandle,(void*)genCamTriggerAd,&genCam,sizeof(int),0);
	if(genCam == 3){
	genCamSetup();
	}
	
	//go into slowmotion if out of generals view
	if(genCam == 1){
		WriteProcessMemory(phandle, (void*)(gameSpeedAd),&slowmotion,sizeof(float),0);
	}
}
	
if(perfCounter == 500||perfCounter == 1000){
	
	//WriteProcessMemory(phandle, (void*)(charOrderBaseAd),&attack,sizeof(int),0);
		//Check for chars actual movement state
	ReadProcessMemory(phandle, (void*)(charBaseAd3+charRealMovOff),&realMove,sizeof(int),0);
	if(realMove == 1){
		myst0 = 15;		//allow animations when standing
	}else{
		myst0 = 11;		//forbid them while running/walking
	}
	
	
		
}

if(perfCounter > 1000){
	perfCounter = 0;}
	
	
	
	
	
	
}
	
	













