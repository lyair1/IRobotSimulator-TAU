echo check order of parameters:


echo check order of parameters:config, house_path, algorithm_path
simulator -config test/testEx1 -house_path test/testEx1 -algorithm_path test/AlgorithmsSO 
echo
echo check order of parameters: house_path, config, algorithm_path
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo
echo  check order of parameters: algorithm_path, house_path, config
simulator -algorithm_path test/AlgorithmsSO -house_path test/testEx1 -config test/testEx1
echo
echo
echo check order of parameters: config, algorithm_path, house_path
simulator -config test/testEx1 -algorithm_path test/AlgorithmsSO -house_path test/testEx1
echo
echo
echo check order of parameters: house, algorithm, config
simulator  -house_path test/testEx1  -algorithm_path test/AlgorithmsSO -config test/testEx1
echo
echo
echo check relative path
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo
echo check relative path with trailing slash
simulator  -house_path test/testEx1/ -config test/testEx1/ -algorithm_path test/AlgorithmsSO/
echo
echo
echo check missing all parameters
simulator
echo
echo
echo check missing algo parameter
simulator  -house_path test/testEx1 -config test/testEx1
echo
echo
echo check missing config parameter
simulator  -house_path test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo
echo check missing house parameter
simulator  -config test/testEx1 -algorithm_path test/AlgorithmsSO
echo
echo
echo check only algo parameter
simulator  -algorithm_path test/AlgorithmsSO
echo
echo
echo check only config parameter
simulator  -config test/testEx1
echo
echo
echo check only house parameter
simulator  -house_path test/testEx1
echo
echo
echo HOUSE TESTS same house should get same score
simulator -house_path test/HouseTests -config test/testEx1

echo
echo
echo check house folder is empty of houses
simulator -house_path test/HouseTests/emptyHouse -config test/testEx1 test/AlgorithmsSO
echo
echo
echo check house with no docking
simulator -house_path test/HouseTests/HouseWithNoDocking -config test/testEx1 test/AlgorithmsSO
echo
echo
echo check house with too many docking
simulator -house_path test/HouseTests/TooManyDocking -config test/testEx1 test/AlgorithmsSO
echo
echo
echo check house with negaive lines
simulator -house_path test/HouseTests/NegativeLine -config test/testEx1 test/AlgorithmsSO
echo
echo
echo check house with docking on wall 
simulator -house_path test/HouseTests/DockingOnWall -config test/testEx1 test/AlgorithmsSO

echo
echo
echo config regular
simulator  -house_path test/testEx1 -config test/ConfigTests/1 -algorithm_path test/AlgorithmsSO
echo
echo
echo config missing 1 arg
simulator  -house_path test/testEx1 -config test/ConfigTests/2 -algorithm_path test/AlgorithmsSO
echo
echo
echo config missing 2 args
simulator  -house_path test/testEx1 -config test/ConfigTests/3 -algorithm_path test/AlgorithmsSO
echo
echo
echo config missing 3 args
simulator  -house_path test/testEx1 -config test/ConfigTests/4 -algorithm_path test/AlgorithmsSO
echo
echo
echo config missing 4 args
simulator  -house_path test/testEx1 -config test/ConfigTests/5 -algorithm_path test/AlgorithmsSO
echo
echo
echo config mixed parameters order
simulator  -house_path test/testEx1 -config test/ConfigTests/6 -algorithm_path test/AlgorithmsSO
echo
echo config file cannot be opened
simulator  -house_path test/testEx1 -config test/ConfigTests/7 -algorithm_path test/AlgorithmsSO
echo
echo
echo test trail house
simulator  -house_path test/testTrailHouse -config test/testTrailHouse -algorithm_path test/AlgorithmsSO
echo
echo
echo test 3 algorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/ThreeAlgorithms
echo
echo
echo test 4 algorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/FourAlgorithms
echo
echo
echo test 2 algorithms
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/TwoAlgorithms
echo
echo
echo test empty algorithm folder
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests
echo
echo
echo
echo test algorithm folder with empty so files
simulator  -house_path test/testEx1 -config test/testEx1 -algorithm_path test/AlgorithmTests/emptySO
echo
echo

exit