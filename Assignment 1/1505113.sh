#!/bin/bash
unzip submissionsAll.zip
rm submissionsAll.zip

IFS=$'\n'
touch Marks1.txt
touch Absent1.txt
StudentIdList=$(cut CSE_322.csv -d ',' -f1 | sed 's/"//g' | sed 's/	//g')
StudentNameList=$(cut CSE_322.csv -d ',' -f2)
#echo $StudentIdList
#echo $StudentNameList

#student id's in an array sid
i=0;
for j in $StudentIdList
do
    sid[i]=$j
    i=`expr $i + 1`
done

#student names in an array sname
i=0;
for j in $StudentNameList
do
    sname[i]=$j
    i=`expr $i + 1`
done

#making the presentlist where present students gets 1 and absent students get 0
k=0

for i in $StudentIdList
do 

    presentlist[k]=0

    for j in $(ls)
    do 
        if [[ $j == *"$i"* ]];then
            presentlist[k]=1
            break
        fi
    done
k=`expr $k + 1`
done


#creating directory "Output" and inside it "Extra"
if [ ! -d "$Output" ]; then
    mkdir Output
    cd Output
    mkdir Extra
    cd ..
else
    cd Output
    if [ ! -d "$Extra" ]; then
        mkdir Extra
        cd ..
    else 
        cd ..
    fi
fi




#main working start.Manipulation of the zip files
for i in $(ls)
do
    if [[ $i == *".zip" ]];then
       zipfilename=$i
       mkdir tempdir
       unzip $zipfilename -d tempdir
       cd tempdir
       
       c=0
       for f in $(ls)
       do
          c=`expr $c + 1`
       done
       
        
       #found single directory 
       if [ $c -eq 1 ]; then
           
           flag=0

           for j in $(ls)
           do
                foldername=$j
           done
            
           #perfect submissions 
           for((k=0;k<${#sid[@]};k++))
           do
           if [ ${sid[$k]} == $foldername ]; then
                flag=1
                echo  "${sid[$k]} 10" >> ../Marks1.txt
                mv $foldername ../Output
                break
           fi
           done

           
           #folder name has "1505113_CSE322_Lab" format.getting 5 marks1
           if [ $flag == 0 ]; then
              for((k=0;k<${#sid[@]};k++))
              do
                 temp=${sid[$k]}
                 if [[ $foldername == *"$temp"* ]]; then
                    flag=1
                    echo  "${sid[$k]} 5" >> ../Marks1.txt
                    mv $foldername $temp
                    mv $temp ../Output
                    break                   
                 fi

              done
           fi

           
           #no student id on the folder name. Have to retrive it from the zip file name and student id list
           if [ $flag == 0 ]; then
              for((k=0;k<${#sid[@]};k++))
              do
                temp=${sid[$k]}
                if [[ $zipfilename == *"$temp"* ]];then
                   flag=1
                   echo "${sid[$k]} 0" >> ../Marks1.txt
                   mv $foldername $temp
                   mv $temp ../Output
                   break
                fi
              done
            fi
            

            #no student id. Trying to retrive name from zip file name 
            if [ $flag == 0 ]; then           
                c=0
                tempid=0
                tempzipfilename=${zipfilename,,}

                for((k=0;k<${#sname[@]};k++))
                do
                    name=${sname[$k]}
                    tempname=${name,,} 
                    if [ presentlist[$k] == 0 ]; then
                        if [[ $tempzipfilename == *"$tempname"* ]];then
                            tempid= ${sid[$k]}
                            c=`expr $c + 1`
                        fi
                    fi
                done
                
                #count 1 means only one person has submitted using his name. His file will be sent to Output and he will get 0 
                if [ $c == 1 ];then
                    $flag=1
                    
                    for((k=0;k<${#sid[@]};k++))
                    do
                        if [ ${sid[$k]} == $tempid ]; then
                            presentlist[k]=1
                            break
                        fi
                    done

                    echo "$tempid 0" >> ../Marks1.txt
                    mv $foldername $tempid
                    mv $tempid ../Output
                fi    

            fi
            
            #unditermined students file. These will be sent to Extra
            if [ $flag == 0 ]; then
                tempname=$zipfilename+" ex"
                mv $foldername $tempname
                mv $tempname ../Output/Extra
            fi





        #found multiple directory ...so new work 
        else

            flag=0

            for((k=0;k<${#sid[@]};k++))
            do
                temp=${sid[$k]}
                if [[ $zipfilename == *"$temp"* ]]; then
                    flag=1
                    mkdir $temp                   
                    for eachdir in $(ls)
                    do
                        if [ $eachdir != $temp ]; then
                            mv $eachdir $temp
                        fi
                    done        

                    echo "${sid[$k]} 0" >> ../Marks1.txt
                    mv $temp ../Output
                    break
                fi  
            done

           
            #no student id. Trying to retrive name from zip file name. same as single directory last 2 operations.
            if [ $flag == 0 ]; then           
                c=0
                tempid=0
                tempzipfilename=${zipfilename,,}

                for((k=0;k<${#sname[@]};k++))
                do
                    name=${sname[$k]}
                    tempname=${name,,} 
                    if [ presentlist[$k] == 0 ]; then
                        if [[ $tempzipfilename == *"$tempname"* ]];then
                            tempid= ${sid[$k]}
                            c=`expr $c + 1`
                        fi
                    fi
                done

                if [ $c == 1 ];then
                    $flag=1
                    for((k=0;k<${#sid[@]};k++))
                    do
                        if [ ${sid[$k]} == $tempid ]; then
                            presentlist[k]=1
                            break
                        fi
                    done
                    
                    echo "$tempid 0" >> ../Marks1.txt
                    mkdir $tempid                   
                    for eachdir in $(ls)
                    do
                        if [ $eachdir != $tempid ]; then
                            mv $eachdir $tempid
                        fi
                    done 
                    
                    mv $tempid ../Output

                fi    

            fi


            if [ $flag == 0 ]; then
                tempid=$zipfilename+" ex"
                mkdir $tempid                   
                for eachdir in $(ls)
                do
                    if [ $eachdir != $tempid ]; then
                        mv $eachdir $tempid
                    fi
                done 
                    
                mv $tempid ../Output/Extra
                
            fi

        fi

        cd ..
        rm -r tempdir
        rm -r $zipfilename
    fi
done


#preparing the Marks and Absent lists
for((i=0;i<${#presentlist[@]};i++))
do
    if [ ${presentlist[$i]} == 0 ];then
        temp=${sid[$i]}
        
        echo "$temp 0" >> Marks1.txt
        echo "$temp" >> Absent1.txt
    fi
done       


touch Marks.txt
touch Absent.txt
sort -n Marks1.txt >> Marks.txt 
sort -n Absent1.txt >> Absent.txt
rm Marks1.txt
rm Absent1.txt
mv Marks.txt Output
mv Absent.txt Output



















