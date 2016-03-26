# IRobotSimulator-TAU
IRobot simulator project for Tel Aviv University programming project

External Libraries use:

1. 	Boost_1_50_0 - get it here: http://sourceforge.net/projects/boost/files/boost/1.50.0/ 
	follow installation details:
	
	Run the following commands from: "VS2013 Native Tools-Command Prompt" (instrucations: http://stackoverflow.com/questions/21476588/where-is-developer-command-prompt-for-vs2013)
	
	1. Unpack the release. On the command line, go to the root of the unpacked tree.
	2. Run either .\bootstrap.bat (on Windows), or ./bootstrap.sh (on other operating systems).
	3. Run
			./b2 install --prefix=PREFIX
			where PREFIX is a directory where you want Boost.Build to be installed.
			Optionally, add PREFIX/bin to your PATH environment variable.