In almost all projects with LCD display you needto create a convinient menu for configuration of devicesand launching of required applications. And it would be nice to have a ready tool for creating it. This tool enables you to concentrate on designing of the final product, not spending resources on designing interface configurator. And this article is about this tool. The project Menu OS was established in August 2010, and since that time it has been used in designing of devices of controlling and automation. Firstly the project was conceived as an open-source, but at the same time, there was an older closed version. Honestly, almost all functionality of the closed version gradually moved into the opened branch and this version of Menu OS is the most actual and constantly supported. License of the project BSD allows to create on it's basis commercial firmware. Menu OS is free, but some closed moduls are purchased and developed for a fee. 
# The structure of the system: #

File is the basic unit of the system. File is characterized by a type, name and a set of 
parameters.  At the moment the system has 6 types of files: 
Type Description
T_FOLDER Folder with a list of files.
T_DFOLDER  Dynamic folder.Stores several copies of the same file.
T_SFOLDER   Selection folder . Stores a list of selection items of config.
T_APP Application.
T_CONF Config. Redaction of config is done the editor of configs and allows to set the required value.
T_DCONF Dynamic config. It is used in conjunction with a dynamic folder and allows to edit the memory configs.
T_SCONF Selection config. It is used in conjunction with a selection folder . It allows one of the several text variants. Each file can have up to two parameters, that define the various properties of the file. More datailed description of the parameters is described in the partition The structure of the files. Total types of files may be up to 256.
The number of files in the menu structure is limited by the amount of the memory controller. At the moment all the variables are used by uint8_t tepe, that requires the number of the files up to 255. 
Given that all files can be dynamic, the total number of the files can be equal to 10^616.
The main features of the system:
1. Tree, intertwined menu with 255 leaves.
2.  Up to 256 different types of files.
3.  Up to 255 levels of nesting.
4. At the moment of using dynamic folders, description of a tree having 10^616 leaves is possible.
5. The lack of hardware bindingto a particular type of MC. Any ATmega controllers with enough memory are supported.
6.  Ability to connect up to 8 buttons and use them in any configuration.The initial customizationof the project


In order to implement this system in your project, it is necessary to carry out the following steps: 
1. To configure buttons.
2. To configure display.
3. To configure an array of files.
4. To configure an array of configs.
5. To connect your applications.
The customization of the project is completed ! You can try to compile the project 
and pour the firmware in the controller.