echo "Menu"
echo "1. Create employee record"
echo "2. Delete employee record"
echo "3. Search employee record"
echo "4. Sort details"
echo "5. List all records"
echo "6. Exit"


while true
do
	read -p "Enter your choice (1-6):" choiceNumber

	case $choiceNumber in
        	1)
                	function createEmployee(){
                        	echo "Enter name of employee :"
                        	read ename
	                        echo "Enter the IdNumber of employee: "
        	                read eid
                	        echo "Enter the salary of employee: "
                        	read sal
	                        if [[ ! $eid =~ ^[0-9]+$ ]]; then
        	                        echo "Please ente the valid employee ID: "
                	                read eid
                        	fi
                        	echo "$eid => $ename, $sal" >> emp.txt
                        	echo "Employee Record for $ename created"

                	}

                	createEmployee
        	;;
        	2)
                	function deleteEmployee(){
                        	echo "Enter the employee id to be deleted"
	                        read eid
        	                ed = $(grep "$eid" emp.txt)
                	        if [ "$ed" ]; then
                        	        sed -i "/$eid/d" emp.txt
                                	echo "Employee details deleted successfully"
				else
                                	echo "Employee Not Found"
				fi
                	}
	
        	        deleteEmployee

        	;;
        	3)
                	function searchEmployee(){
                        	echo "Enter the Employee ID number to be searched"
	                        read eid
        	                ed = $(grep "$eid" emp.txt)
                	        if [ "$ed" ]; then
                        	        echo "$ed"
	                        else
        	                        echo "Employee Details are not found"
                	        fi
                	}
                	searchEmployee
        	;;
        	4)
                	function sortData(){
                        	 sort -V emp.txt 
                	}
                	sortData
        	;;
        	5)
                	function display(){
                        	if [ -f "$1" ]; then
                                	cat "$1"
                        	fi
                	}
                	display emp.txt
        	;;
		6)
			break
		;;
		*)
			echo -n "Invaild"
		;;
	esac
done

