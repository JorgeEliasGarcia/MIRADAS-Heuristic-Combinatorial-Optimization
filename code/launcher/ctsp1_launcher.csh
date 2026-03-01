#!/bin/csh

# echo "Compiling ctsp1_launcher.csh"

# sh /home/jorge/code/practicas/TFG/CTSP1_heuristic/compile.sh

    alias acalc '       awk "BEGIN{ print \!* }" '

    set fecha  = `date "+%y%m%d%H%M%S"`
	set fechab = `date "+%d/%m/%Y %H:%M:%S"`

	set basePath      = ./
	set baseExePath   = ${basePath}build/main/
   	set programPath   = ${baseExePath}
	set solverExe     = ${programPath}ctsp1_heuristic

    set inputPath     = ${basePath}instances/SubramanyamGounaris/
	set outputPath    = ${basePath}output/

# Instancias: burma14 ulysses16 br17 gr17 gr21 ulysses22 gr24 fri26 bayg29 bays29 ftv33 ftv35 ftv38 dantzig42 swiss42 p43 ftv44 ftv47 att48 gr48 hk48 ry48p eil51 ft53 ftv55 brazil58 ftv64 ft70 ftv70 st70 eil76 pr76 gr96 rat99 kroA100 kroB100 kroC100 kroD100 kroE100 rd100 eil101
# Petan: p43 gr48 ry48p brazil58 berlin52

	set instanceList  = ( brazil58 )
	
	echo Starting: ${fechab}
	echo " "

	set solverId =  1

	set outputPath    = ${outputPath}CTSP1_${fecha}/algorithm_${solverId}/
	set outputSta     = ${outputPath}sta/
	set outputStaFile = ${outputSta}CTSP1_stat.txt

	mkdir -p ${outputPath}
	mkdir -p ${outputSta}

	foreach familyName ( ${instanceList} )

		echo " "

		echo Processing family: ${familyName}

		set familyPath = ${outputPath}${familyName}/

		mkdir -p ${familyPath}

		set outputLog     = ${familyPath}log/
		set outputSol     = ${familyPath}sol/

		mkdir -p ${outputLog}
		mkdir -p ${outputSol}

		echo " "

		foreach targetFile (${inputPath}${familyName}*.contsp)

			set targetName   = $targetFile:t:r

			echo "   File:" ${targetName}

				set instanName    = $targetName:t:r
				set logFileName   = ${outputLog}${targetName}.log
				set solFileName   = ${outputSol}${targetName}.txt
				set feaFileName   = ${outputSol}${targetName}.sol

				echo "      Tar File: " ${targetFile}
				echo "      Sta File: " ${outputStaFile}
				echo "      Log File: " ${logFileName}
				echo "      Sol File: " ${solFileName}

				${solverExe} ctsp1 ${targetFile} ${outputStaFile}  ${logFileName} ${solFileName} ${feaFileName} ${solverId}		

		end

	end

	set fechae = `date "+%d/%m/%Y %H:%M:%S"`

	echo " "
	echo Ending: ${fechae} 	


