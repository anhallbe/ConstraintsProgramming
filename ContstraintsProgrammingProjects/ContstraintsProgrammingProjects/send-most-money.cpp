/*
	Send most money test!
	send-most-money.cpp
*/

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

using namespace Gecode;

class SendMostMoney : public Space {
protected:
	IntVarArray l;
public:
	SendMostMoney(void) : l(*this, 8, 0, 9) {
		IntVar s(l[0]), e(l[1]), n(l[2]), d(l[3]),
				m(l[4]), o(l[5]), r(l[6]), y(l[7]);

		//NO LEADING ZEROS
		rel(*this, s, IRT_NQ, 0);
		rel(*this, m, IRT_NQ, 0);

		//ALL LETTERS DISTINCT
		distinct(*this, l);

		//LINEAR EQUATION
		IntArgs c(4+4+5);	IntVarArgs x(4+4+5);
		c[0] = 1000;	c[1] = 100;	c[2] = 10;	c[3] = 1;
		x[0] = s;		x[1] = e;	x[2] = n;	x[3] = d;
		c[4] = 1000;	c[5] = 100;	c[6] = 10;	c[7] = 1;
		x[4] = m;		x[5] = o;	x[6] = r;	x[7] = e;
		c[8]=-10000;		c[9]=-1000;	c[10]=-100;	c[11]=-10;	c[12]=-1;
		x[8]=m;			x[9]=o;		x[10]=n;	x[11]=e;	x[12]=y;
		linear(*this, c, x, IRT_EQ, 0);

		//POST BRANCHING
		branch(*this, l, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	}

	//SEARCH SUPPPORT
	SendMostMoney(bool share, SendMostMoney& s) : Space(share, s) {
		l.update(*this, share, s.l);
	}

	virtual Space* copy(bool share) {
		return new SendMostMoney(share, *this);
	}

	//PRINT SOLUTION
	void print(void) const {
		std::cout << l << std::endl;
	}

	void print(std::ostream& os) const {
		os << l << std::endl;
	}

	virtual void constrain(const Space& _b) {
		const SendMostMoney& b = static_cast<const SendMostMoney&>(_b);
		IntVar e(l[1]), n(l[2]), m(l[4]), o(l[5]), y(l[7]);
		IntVar b_e(b.l[1]), b_n(b.l[2]), b_m(b.l[4]), b_o(b.l[5]), b_y(b.l[7]);
		int money = (10000*b_m.val()+1000*b_o.val()+100*b_n.val()+
                 10*b_e.val()+b_y.val());
	IntArgs c(5); IntVarArgs x(5);
    c[0]=10000; c[1]=1000; c[2]=100; c[3]=10; c[4]=1;
    x[0]=m;     x[1]=o;    x[2]=n;   x[3]=e;  x[4]=y;
    linear(*this, c, x, IRT_GR, money);
	}
};

//MAIN
int main(int argc, char* argv[]) {
	////CREATE MODEL AND SEARCH ENGINE
	//SendMostMoney* m = new SendMostMoney;
	//DFS<SendMostMoney> e(m);
	//delete m;
	////SEARCH AND PRINT ALL SOLUTIONS
	//while(SendMostMoney* s = e.next()) {
	//	s->print();
	//	delete s;
	//}
	//system("pause");
	//return 0;

	//SendMostMoney* m = new SendMostMoney;
	//Gist::dfs(m);
	//delete m;
	//return 0;

	SendMostMoney*m = new SendMostMoney;
	Gist::Print<SendMostMoney> p("Print solution");
	Gist::Options o;
	o.inspect.click(&p);
	Gist::dfs(m,o);
	delete m;
	return 0;
}
