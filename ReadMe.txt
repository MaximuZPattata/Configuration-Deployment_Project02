<<-----------------------------------------------------------------------------------JsonXMLConfig------------------------------------------------------------------------------------------------------->>

- About the project : 
	-It is a simple project. The program displays how to read and write from a Json as well as XML file. The project's user interface allows user to read the contents of the file UserPreferences.json/ UserPreferences.xml and also write to it. It also enables the user to change between two different languages(English and French). 

- How to build : 
	-The solution from the path "JsonXMLConfig/JsonXMLConfig.sln" needs to be opened and built(either in debug or release). Once the solution has been built successfully(it probably should), the solution will now run and the userInterface provides the user with control options.
	- Also the user can switch between Json and Xml by commenting the code #define USE_JSON_CONFIG or #define USE_XML_CONFIG in the main(). 
	- In the user interface, the user can use keys like [1,2,3 and 4] to type or choose an option related to user preference. 'X' or 'x' is pressed to exit the program.

- Limitations : 
	- The one known limitation is that when writing in the console, typing out a user preference section, the program doesnt take strings with spaces, for example "Hello World" is only taken as "Hello". I further researched and figured that the root cause might be "std::cin>>" and I also tried using "std::getline()". But it caused more problems. So I sticked it "std::cin>>" for now. So please avoid spaces :)