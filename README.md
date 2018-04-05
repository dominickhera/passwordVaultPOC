# passwordVaultPOC
A password vault proof of concept application utilizing hash tables to store user information (NOT ENCRYPTED FOR ACTUAL USE)

dominick hera

to run
=

./bin/run

WS keys to move up and down menu

to compile
=

type 'make' to run the makeFile

description
=
- the program is a basic password vault program that is designed to store your information for various user accounts. It uses a config file that if it doesnt exist, will create a new one for the user that will automatically be called upon launching the program over again. the application asks for the master password before accessing each option just for an added level of security. the program contains a nice border and clean simple UI that the user is able to navigate using the W and S keys for up and down the menu, respectively. it maintains a level of simplicity that the user is unable to get confused or lost over. upon exiting the program, the data file will automatically be saved so the user doesn't need to worry about the data file after setup.

known limitations
=
- is set up to create a new data file upon initial launch, but doesnt allow for a replacement of that file unless the config file is deleted and the application is relaunched.
