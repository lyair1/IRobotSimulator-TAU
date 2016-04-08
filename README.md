# IRobotSimulator-TAU
IRobot simulator project for Tel Aviv University programming project

External Libraries use:

1. 	Boost_1_50_0 - get it here: http://sourceforge.net/projects/boost/files/boost/1.57.0/ 
	follow installation details:
	
	Run the following commands from: "VS2013 Native Tools-Command Prompt" (instrucations: http://stackoverflow.com/questions/21476588/where-is-developer-command-prompt-for-vs2013)
	
	1. Unpack the release. On the command line, go to the root of the unpacked tree.
	2. Run either .\bootstrap.bat (on Windows), or ./bootstrap.sh (on other operating systems).
	3. Run
			./b2 install --prefix=boostInstall --build-type=complete -link=static
	4. Run bjam -j4 --debug-symbols=on --build-type=complete toolset=msvc-12.0 threading=multi runtime-link=shared address-model=32
		
		
		# msvc-12.0 for VS2013
	
	5. Edit IRobotSimulatotTAU.VCXproj to match your boost install path:
	   <AdditionalIncludeDirectories>C:\Users\yairle\Downloads\boost_1_57_0\boost_1_57_0\boostInstall\include\boost-1_57;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
       <AdditionalUsingDirectories>C:\Users\yairle\Downloads\boost_1_57_0\boost_1_57_0\boostInstall\lib;%(AdditionalUsingDirectories)</AdditionalUsingDirectories>
	   <AdditionalLibraryDirectories>C:\Users\yairle\Downloads\boost_1_57_0\boost_1_57_0\stage\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
	   
	  


	