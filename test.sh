#!/bin/bash

# COLORS VAR

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[1;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m'

echo -ne "${CYAN}"
echo "    __  ______    __    __    ____  _________  __________  ____ "
echo "   /  |/  /   |  / /   / /   / __ \\/ ____/   |/_  __/ __ \\/ __ \\"
echo "  / /|_/ / /| | / /   / /   / / / / /   / /| | / / / / / / /_/ /"
echo " / /  / / ___ |/ /___/ /___/ /_/ / /___/ ___ |/ / / /_/ / _, _/ "
echo "/_/  /_/_/  |_/_____/_____/\\____/\\____/_/  |_/_/  \\____/_/ |_|  "
echo -e "${NC}"
echo 'By tmatis <tmatis@student.42.fr>'
echo

if [ ! -n "$BASH" ] ;
then
	echo "Please run this script using bash"
	exit 1
fi

rm -rf ./logs
mkdir ./logs

echo -ne "${BOLD}Make malloc hook ... ${NC}"

make re > ./logs/make_malloc_hook.log
return_value=$?

if [ $return_value -ne 0 ]; then
	echo -e "${RED}fail${NC} check ./ft_mallocator/logs/make_malloc_hook.log"
	exit 1
else
	echo -e "${GREEN}done${NC}"
fi

echo -ne "${BOLD}Compiling ... ${NC}"

clang -Wall -Werror -Wextra -fsanitize=undefined -rdynamic -g main.c -o test_bin -L. -lmallocator

echo -e "${GREEN}done${NC}"

echo -ne "${BOLD}Fetching malloc routes ... ${NC}"

./test_bin &> ./logs/fetch_routes.log

echo -e "${GREEN}done${NC}"

echo

routes=()

readarray -t routes < ./routes.tmp

for route in "${routes[@]}"
do
	# we need to parse route the format is :
	# addrr:name addr:name addr:name #iteration

	#parse the iteration
	iteration=$(echo $route | cut -d '#' -f 2)

	#get the addresses
	addresses_names=$(echo $route | cut -d '#' -f 1)

	names=""
	addresses=""
	for address_name in $addresses_names
	do
		names="$names $(echo $address_name | cut -d ':' -f 2)"
		addresses="$addresses $(echo $address_name | cut -d ':' -f 1)"
	done

	size_names=$(echo $names | wc -w)
	echo -ne "${BOLD}Testing route: ${NC}"
	for name in $names
	do
		echo -ne "${CYAN}$name${NC}"
		# if its not the last one, we need to add a space
		if [ $((size_names--)) -ne 1 ]; then
			echo -ne "${YELLOW} <- ${NC}"
		fi
	done
	echo " ..."

done
