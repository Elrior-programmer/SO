#include <ncurses.h>
#include <cstdlib>
class Philosopher;

enum Frok_state {
	LAYING,
	HOLD,
	IN_USE
};

enum Philosopher_status {
	THINKING,
	WAITING,
	EATING
};

struct Fork
{
	int id;
	Frok_state used;
	Philosopher* user;
	public:
		Fork() : id(0), used(LAYING), user(nullptr) {}
		Fork(int id_) : id(id_), used(LAYING), user(nullptr) {}
		Fork(int id_, Frok_state is_being_used, Philosopher* user_) : id(id_), used(is_being_used), user(user_) {}
	int get_id() const {
		return this->id;
	}
};

struct Philosopher {
	int id;
	Fork* left_fork;
	Fork* right_fork;
	Philosopher_status status;
		Philosopher(int id_=0) {
			this->id = id_;
			this->left_fork = nullptr;
			this->right_fork = nullptr;
			this->status = THINKING;
		}
		Philosopher(int id_, Fork* left_fork_, Fork* right_fork_, Philosopher_status stauts_)
		: id(id_), left_fork(left_fork_), right_fork(right_fork_), status(stauts_) {}
	int get_id() const {
		return this->id;
	}
};

void visualization(Philosopher philosopher_table[], int amount_of_philosophers) {
	initscr();			/* Start curses mode 		  */
	for(int i = 0 ; i < amount_of_philosophers ; i++) {
		printw(" | Filozof %d",philosopher_table[i].get_id());
		printw(" |");
	}
	printw("\n ");
	for(int i = 0 ; i < amount_of_philosophers ; i++) {
		if (philosopher_table[i].status == 0)
		{
			printw("| THINKING  | ");
		}
		else if (philosopher_table[i].status == 1) {
			printw("|  WATING   | ");
		}
		else {
			printw("|  EATING   | ");
		}
		
	}
	printw("\n");
	for(int i = 0 ; i < amount_of_philosophers ; i++) {
		printw(" | R_F%d L_F%d |",philosopher_table[i].right_fork->get_id(),philosopher_table[i].left_fork->get_id());
	}
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */
}

int main(int argc, char* argv[])
{	
	if(argc < 2 ) {
		printf("zla ilosc parametrow - zrestartuj program\n");
		return 1;
	}
	else {
		
		int amount_of_philosophers = std::atoi(argv[1]);
		Philosopher philosopher_table[amount_of_philosophers];
		Fork fork_table[amount_of_philosophers];
		for (int i = 0 ; i < amount_of_philosophers ; i++)
		{
			fork_table[i] = Fork(i,LAYING,nullptr);
		}
		philosopher_table[0] = Philosopher(0,&fork_table[0],&fork_table[amount_of_philosophers-1], EATING);
		for(int i = 1 ; i < amount_of_philosophers ; i++) {
			philosopher_table[i] = Philosopher(i, &fork_table[i],&fork_table[i+1],WAITING);
		}
		
		visualization(philosopher_table,amount_of_philosophers);
		return 0;
	}
}