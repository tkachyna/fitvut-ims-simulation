# *
# *   IMS Project - solar collectors and their efficiency
# *   
# *   @author Tadeáš Kachyňa, <xkachy00@stud.fit.vutbr.cz>
# *   @date 4/12/2022
# *   @file Makefile
# *


all:
	g++ -std=c++14 -pedantic -Wall -Werror -Wextra solarcollector.cpp data.cpp -o collector
run:
	@./collector -a 45 -s 45
	@./collector -a 45 -s 60
	@./collector -a 45 -s 90
clean:
	rm -rvf *.o collector