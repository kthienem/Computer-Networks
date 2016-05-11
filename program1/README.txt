The file named "udpclient.cpp" is the only file needed to run the sample UDP
client. To compile the code type the command "g++ udpclient.cpp -o udpclient".
To then run the code type "./udpclient studentXX.cse.nd.edu 41500 'Filename/text'"
The XX represents the number of the student machine being connected to. For this 
program please enter 02 or 03 to contact a server with the working server code.
The file name/ text wishing to sent can be anything. The program will check if the
text given for the third argument is an openable file. If it is then it will send
the text in the file. If it is not then it will take the argument and send that 
string.
