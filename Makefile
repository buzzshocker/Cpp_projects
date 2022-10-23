all: test_treemap anitaborg_donations

test_treemap: test_treemap.cc treemap.h
	g++ -Wall -Werror -o -std=c++11 test_treemap.cc -o test_treemap -pthread -lgtest

anitaborg_donations: anitaborg_donations.cc
	g++ -Wall -Werror -std=c++11 anitaborg_donations.cc -o anitaborg_donations

clean:
	rm -rf anitaborg_donations test_treemap create_dataset *.dat *.out *.o