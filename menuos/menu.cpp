/*Copyright (c) 2013, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include "menu.h"
#include "static.h"    //файл определений
#include "motionview.h"
#include "drive.h"

uint8_t brCrumbs[MAXDEPTH+1][3]={0};//массив хлебных крошек. двухмерный 1 байт - номер программы, второй - положение курсора
uint8_t level=0;//номер текущего уровня
uint8_t fileNumb=0;//Номер текущего файла
uint8_t fileData[4]; //а в этой переменной будем хранить параметры элемента.
uint8_t Positions[4]={0};//позиции строк. X,Y начальные, X,Y текущие
char FileName[18];

//считывает данные ткущей папки
void MenuSetup(){
	BSlot(BUTTONSLOTLEFT,MenuButtonLeft);//листать  влево
	BSlot(BUTTONSLOTRIGHT,MenuButtonRight);//листать  влево
	BSlot(BUTTONSLOTUP,MenuButtonUp);//листать  влево
	BSlot(BUTTONSLOTDOWN,MenuButtonDown);//листать  влево
	BSlot(BUTTONSLOTRETURN,MenuButtonReturn);//листать  влево
	BSlot(BUTTONSLOTENTER,MenuButtonEnter);//листать  влево		
	Positions[0]=POSXMIN;
	Positions[1]=POSXMAX;
	GLCD.SelectFont(MENUFONT);
	FileReturn();
}
//устанавливает курсор в необходимую позицию.

void MenuStartUp(){
//запускаем первое приложение
MenuButtonEnter();
}


void DispString(uint8_t str, char* textstr)
{
  if ((str>=0) && (str<DISPSTR)){//ограничим область работы
    GLCD.CursorToXY(POSXMIN+1,POSYMIN+1+str*MENUFOTNHIGHT);//ставим курсор
    GLCD.Puts(textstr);//пишем
  }
}
void MenuHeader(){
	GLCD.CursorToXY(0,0);
	GLCD.Puts(FileName);
	GLCD.DrawHLine(0,8,127,BLACK);

}


//Функция рисует текущие данные
void PageList(){
  GLCD.FillRect(0,0,GLCD.Width,GLCD.Height,WHITE);
  MenuHeader();
  char buff[LCDCOL-5];
  char buff2[LCDCOL-3];
  uint8_t currs;
  uint8_t fstart;
  switch (fileData[0]){
  case T_FOLDER://обычная папка..
    if ((brCrumbs[level][1]&LCDINVBITS)==((fileData[2])&LCDINVBITS)) currs=(fileData[2])&LCDBITS;//Это максимальное положение курсора на последней странице
    else currs=LCDBITS;
    fstart=fileData[1]+(brCrumbs[level][1]&LCDINVBITS);//номер файла для старта вывода.
    for (uint8_t i=0;i<=currs;i++)//пишем из массива для статических папок
    {
 	strlcpy_P(buff, (char*)pgm_read_word(&(fileNames[i+fstart])),sizeof(buff));//прочитали имя
    DispFile(i,i+fstart,buff);
    }
    break;
  case T_DFOLDER://динамическая папка
    if ((brCrumbs[level][1]&LCDINVBITS)==((fileData[2])&LCDINVBITS)) currs=(fileData[2])&LCDBITS;//Это максимальное положение курсора на последней странице
    else currs=LCDBITS; 
    strcpy_P(buff, (char*)pgm_read_word(&(fileNames[fileData[1]])));//читаем общее название
    for (uint8_t i=0;i<=currs;i++)
    {
      sprintf(buff2, "%s %d", buff, (brCrumbs[level][1]&LCDINVBITS)+i);
      DispString(i,buff2);
    }
    break;
  case T_SFOLDER://папка выбора параметра
    uint8_t config_folder_view = pgm_read_byte(&fileStruct[FILEREW*fileData[1]+1]);//читаем байт сдвига.
    uint16_t config_folder_data =  ConfigReadWord(brCrumbs[level][0]+config_folder_view);//чтение значения выбора
    if (config_folder_data>fileData[2]) config_folder_data=fileData[2];
    if (config_folder_view<0) config_folder_view=0;	 
    if (brCrumbs[level][2]==0){
      brCrumbs[level][1]=config_folder_data;//ставим курсор в выбранную позицию.
      brCrumbs[level][2]=1;
    }
    if ((brCrumbs[level][1]&LCDINVBITS)==((fileData[2])&LCDINVBITS)) currs=(fileData[2])&LCDBITS;//Это максимальное положение курсора на последней странице
    else currs=LCDBITS;	
    fstart=fileData[1]+(brCrumbs[level][1]&LCDINVBITS);//номер файла для старта вывода.
    for (uint8_t i=0;i<=currs;i++)//пишем из массива для статических папок
    {
      strlcpy_P(buff, (char*)pgm_read_word(&(fileNames[i+fstart])),sizeof(buff));//прочитали имя
      DispString(i,buff);//пишем из массива
    }
    break;
  }   
	GLCD.InvertRect(0,POSYMIN+(brCrumbs[level][1]&LCDBITS)*MENUFOTNHIGHT,128,MENUFOTNHIGHT);
}

//отображает тот или иной файл
void DispFile(uint8_t str, uint8_t _fnumber, char* _buff){
	uint8_t _FileType=pgm_read_byte(&fileStruct[FILEREW*_fnumber]);//читаем тип файла
	uint8_t _ConfNumb;
	switch (_FileType)//сюда добавляем в зависимости от типа файла.
  {
  //case T_FOLDER:
  //   break;
 // case T_APP:
  //  break;
  case T_CONF:
	_ConfNumb=pgm_read_byte(&fileStruct[FILEREW*_fnumber+1]);//читаем номер конфига 
	if ((str>=0) && (str<DISPSTR))//ограничим область работы
 	 ConfigView(POSXMIN,POSYMIN+str*MENUFOTNHIGHT,_buff,_ConfNumb,0);
    break;
 // case T_DCONF:
 //   break;
 // case T_DFOLDER:
 //   break;
//  case T_SFOLDER:
 //   break;
 // case T_SCONF://обновление конфига
 //   break;
	default:
      DispString(str,_buff);//пишем из массива
	break;
  }
	
}

//Смена указателя вверх
void MenuButtonUp(){
	if (brCrumbs[level][1]>0){ 
		brCrumbs[level][1]--;

	}
    else{ 
		brCrumbs[level][1]=fileData[2];//дошли до начала, начали с конца.	
	}
	PageList();
}

void MenuButtonDown(){
	if (brCrumbs[level][1]<fileData[2])
		brCrumbs[level][1]++;
    else
		brCrumbs[level][1]=0;//не дошли еще до конца? тогда стремимся к нему. Дошли? класс, начинаем сначала
	PageList();
}

void MenuButtonLeft(){
	//нет действия
}

void MenuButtonRight(){
	//временная переадрессация
	MenuButtonEnter();
}

void MenuButtonEnter(){
	if (!fileData[0])//обычная папка. В ней все больно просто
  {
    fileNumb=fileData[1]+brCrumbs[level][1];//берем номер стартового файла и прибавляем сколько мы вниз нащелкали.
  }
  else //динамическая папка тут все еще проще
  {
    fileNumb=fileData[1];//ЖЖОШ!
  }
  if (MAXDEPTH>level){//защита от ухода за пределы массива хлебных крошек.
	level++;//на уровень глубже
  brCrumbs[level][0]=fileNumb;//вошли внутрь? супер, запишем инфу о нашем местоположении.
  brCrumbs[level][1]=0;
  brCrumbs[level][2]=0;
  FileGet(fileNumb);//считываем информацию 
  }  
  switch (fileData[0])//сюда добавляем в зависимости от типа файла.
  {
  case T_FOLDER:
    PageList();
    break;
  case T_APP:
    ApplicationStart(fileData[2]);
    break;
  case T_CONF:
  	//Создаем точку выхода из приложения
	TManager.WorkType=1;
	TManager.TReload(3,*ApplicationStop,1);//включаем вызов.	
   ConfigSetup(POSXMIN,POSYMIN+(brCrumbs[level-1][1]&LCDBITS)*MENUFOTNHIGHT,FileName,fileData[1],0);
    break;
  case T_DCONF:
  	//Создаем точку выхода из приложения
	TManager.WorkType=1;
	TManager.TReload(3,*ApplicationStop,1);//включаем вызов.	
   ConfigSetup(POSXMIN,POSYMIN+(brCrumbs[level-1][1]&LCDBITS)*MENUFOTNHIGHT,FileName,fileData[1],brCrumbs[level-1][1]);
    break;
  case T_DFOLDER:
    PageList();
    break;
  case T_SFOLDER:
    PageList();
    break;
  case T_SCONF://обновление конфига
    ConfigWriteWord(fileData[1]+brCrumbs[level-1][0],brCrumbs[level-1][1]);
ApplicationStart(0xFA);
    break;
  }
	
}

void MenuButtonReturn(){
	FileReturn();
}

void FileReturn(){
//во первых, удалим текущую информацию о местоположении:
  brCrumbs[level][0]=0;
  brCrumbs[level][1]=0;
  brCrumbs[level][2]=0;
  //вернемся на уровень назад, только если он еще больше нуля. А то уйдем нахер в минуса.
  if (level>0) level--;//находясь на уровне выше, уже можно прочесть инфу о родителе:
  fileNumb=brCrumbs[level][0];//при этом положение курсора не сбрасывается! мы вернемся туда, откуда пришли.
  FileGet(fileNumb);//Читаем инфу о файле.
  PageList();	

}


//функция считывает данные о файле
void FileGet(uint8_t fnumb){
  for (uint8_t i=0;i<FILEREW;i++)
    fileData[i]=pgm_read_byte(&fileStruct[FILEREW*fnumb+i]);//Считываем исловые значения
  strlcpy_P(FileName, (char*)pgm_read_word(&(fileNames[fnumb])),sizeof(FileName));//Считываем название	
	
}

//При работающих приложения функция вызывается из 3 слота диспетчера, проверяет состояние приложения.
void ApplicationStop(){
	if (!TManager.WorkType)//приложение закрывается.
		{
		TManager.TReload(3,*ApplicationStop,0);//Отключаем вызов.
		GLCD.ClearScreen();
		MenuSetup();//Установка кнопок
		}

}

//Все приложения подключаются сюда.
void ApplicationStart(uint8_t AppNumber){
	//Создаем точку выхода из приложения
	TManager.WorkType=1;
	TManager.TReload(3,*ApplicationStop,1);//включаем вызов.	
	switch (AppNumber){
		case 1:
		MotionViewSetup();
			//Основная программа
		break;
		case 2:
			DriveTestSetup();
		break;
		case 0xFA://выводит Saved на экран
		GLCD.FillRect(46,28,36,8,WHITE);
		GLCD.CursorToXY(46,28);
		GLCD.Puts("Saved!");
		TManager.TReload(2,AppSaved,100);//на 1 секунду
		break;
		
	}
}

void AppSaved(){
	TManager.TReload(2,AppSaved,0);
	TManager.WorkType=0;
}