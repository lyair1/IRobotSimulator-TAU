echo check order of parameters:


echo check order of parameters:config, house_path, algorithm_path
echo simulator -config test/testEx1 -house_path test/testEx1 -algorithm_path test/AlgorithmsSO 
simulator -config test/testEx1 -house_path test/testEx1 -algorithm_path test/AlgorithmsSO 
echo
echo

echo check order of parameters: house_path, config, algorithm_path
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo

echo  check order of parameters: algorithm_path, house_path, config
echo simulator -algorithm_path test/AlgorithmsSO -house_path test/testEx1 -config test/testEx1
simulator -algorithm_path test/AlgorithmsSO -house_path test/testEx1 -config test/testEx1
echo
echo

echo check order of parameters: config, algorithm_path, house_path
echo simulator -config test/testEx1 -algorithm_path test/AlgorithmsSO -house_path test/testEx1
simulator -config test/testEx1 -algorithm_path test/AlgorithmsSO -house_path test/testEx1
echo

echo check order of parameters: house, algorithm, config
echo simulator  -house_path test/testEx1  -algorithm_path test/AlgorithmsSO -config test/testEx1
simulator  -house_path test/testEx1  -algorithm_path test/AlgorithmsSO -config test/testEx1
echo
echo

echo check relative path
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo

echo check relative path with trailing slash
echo simulator  -house_path test/testEx1/ -config test/testEx1/ -algorithm_path test/AlgorithmsSO/
simulator  -house_path test/testEx1/ -config test/testEx1/ -algorithm_path test/AlgorithmsSO/
echo
echo


echo check absolute path with trailing slash
echo simulator  -house_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/testEx1/ -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmsSO/
simulator  -house_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/testEx1/ -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmsSO/
echo
echo

echo check absolute path withOUT trailing slash
echo simulator  -house_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/testEx1 -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmsSO
simulator  -house_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/testEx1 -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmsSO
echo
echo

echo check missing all parameters
echo simulator
simulator
echo
echo

echo check missing algo parameter
echo simulator -house_path test/testEx1 -config test/testEx1
simulator -house_path test/testEx1 -config test/testEx1
echo
echo

echo check missing config parameter
echo simulator  -house_path test/testEx1 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo

echo check missing house parameter
echo simulator  -config test/testEx1 -algorithm_path test/AlgorithmsSO
simulator  -config test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo

echo check only algo parameter
echo simulator  -algorithm_path test/AlgorithmsSO
simulator  -algorithm_path test/AlgorithmsSO
echo
echo

echo check only config parameter
echo simulator  -config test/testEx1
simulator  -config test/testEx1
echo
echo

echo check only house parameter
echo simulator  -house_path test/testEx1
simulator  -house_path test/testEx
echo
echo

echo check house folder is empty of houses
echo simulator -house_path test/HouseTests/emptyHouse -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/emptyHouse -config test/testEx1 test/AlgorithmsSO
echo
echo

echo check house with no docking
echo simulator -house_path test/HouseTests/HouseWithNoDocking -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/HouseWithNoDocking -config test/testEx1 test/AlgorithmsSO
echo
echo

echo check house with too many docking
echo simulator -house_path test/HouseTests/TooManyDocking -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/TooManyDocking -config test/testEx1 test/AlgorithmsSO
echo
echo

echo check house with negaive lines
echo simulator -house_path test/HouseTests/negativeLine -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/negativeLine -config test/testEx1 test/AlgorithmsSO
echo
echo

echo check ALL BAD HOUESE house with too many docking and no docking 
echo simulator -house_path test/HouseTests/AllBadDocking -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/AllBadDocking -config test/testEx1 test/AlgorithmsSO
echo
echo

echo check house with too many docking and no docking 
echo simulator -house_path test/HouseTests/BadDocking -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/BadDocking -config test/testEx1 test/AlgorithmsSO
echo
echo

echo check house with docking on wall 
echo simulator -house_path test/HouseTests/DockingOnWall -config test/testEx1 test/AlgorithmsSO
simulator -house_path test/HouseTests/DockingOnWall -config test/testEx1 test/AlgorithmsSO
echo
echo

echo config regular
echo simulator  -house_path test/testEx1 -config test/ConfigTests/1 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/1 -algorithm_path test/AlgorithmsSO
echo
echo

echo config missing 1 arg
echo simulator  -house_path test/testEx1 -config test/ConfigTests/2 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/2 -algorithm_path test/AlgorithmsSO
echo
echo

echo config missing 2 args
echo simulator  -house_path test/testEx1 -config test/ConfigTests/3 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/3 -algorithm_path test/AlgorithmsSO
echo
echo

echo config missing 3 args
echo simulator  -house_path test/testEx1 -config test/ConfigTests/4 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/4 -algorithm_path test/AlgorithmsSO
echo
echo

echo config missing 4 args
echo simulator  -house_path test/testEx1 -config test/ConfigTests/5 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/5 -algorithm_path test/AlgorithmsSO
echo
echo

echo config bad :config.ini having bad values for 2 parameters BatteryCapacity BatteryRechargeRate
echo simulator  -house_path test/testEx1 -config test/ConfigTests/8 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/8 -algorithm_path test/AlgorithmsSO
echo
echo

echo config mixed parameters order
echo simulator  -house_path test/testEx1 -config test/ConfigTests/6 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/6 -algorithm_path test/AlgorithmsSO
echo
echo

echo config file cannot be opened
echo simulator  -house_path test/testEx1 -config test/ConfigTests/7 -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testEx1 -config test/ConfigTests/7 -algorithm_path test/AlgorithmsSO
echo
echo

echo test trail house
echo simulator  -house_path test/testTrailHouse -config test/testTrailHouse -algorithm_path test/AlgorithmsSO
simulator  -house_path test/testTrailHouse -config test/testTrailHouse -algorithm_path test/AlgorithmsSO
echo
echo

echo test 3 algorithms
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/ThreeAlgorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/ThreeAlgorithms
echo
echo

echo test 4 algorithms
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/FourAlgorithms

simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/FourAlgorithms
echo
echo

echo test 2 algorithms
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/TwoAlgorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/TwoAlgorithms
echo
echo

echo test 1 algorithm
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/OneAlgorithm
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/OneAlgorithm
echo
echo

echo test 0 algorithm = empty folder
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/NoAlgorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/NoAlgorithms
echo
echo

echo test algorithms folder doesnt exists
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/ImaginaryFolder
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/ImaginaryFolder
echo
echo

echo test 0 algorithm = empty folder -  absolute path a/home/cc/students/cs/nirorman/irobot3/test/testEx1/
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmTests/NoAlgorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmTests/NoAlgorithms
echo
echo

echo test algorithms folder doesnt exists absolute path
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmTests/ImaginaryFolder
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path /specific/a/home/cc/students/cs/nirorman/irobot3/test/AlgorithmTests/ImaginaryFolder
echo
echo

echo test if algorithms found in working directory
echo simulator  -house_path test/testEx1 -config test/testEx1 
simulator  -house_path test/testEx1 -config test/testEx1 
echo
echo

echo
echo test algorithm folder with empty so files
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/emptySO
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/emptySO
echo
echo

echo
echo test score formula
echo simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula ./
simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula ./
echo

echo test score formula - valid so but no score formula
echo simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/ScoreSObad
simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/ScoreSObad
echo
echo

echo test score formula VALID
echo simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/SOSCORE
simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/SOSCORE
echo
echo

echo test threads 1
echo simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/ScoreSO
simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/SOSCORE -thread 1
echo
echo

echo test threads 2
echo simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/ScoreSO
simulator -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO -score_formula test/SOSCORE -thread 2
echo
echo

echo test OUR HOUSES  complicated test threads 3
echo simulator -house_path test/OurHouses -config test/testEx1 -thread 7
simulator -house_path test/OurHouses -config test/testEx1 -thread 7
echo
echo

echo test HOUSES  complicated test threads 7
echo simulator -house_path test/HouseTests -config test/testEx1 -thread 7
simulator -house_path test/HouseTests -config test/testEx1 -thread 7
echo
echo
exit