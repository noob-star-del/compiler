#include<iostream>
#include<stdio.h>
#include<string>
#include<windows.h>
#include<stdlib.h>
using namespace std;
string next_str;
void error(int n);
void prog(FILE* fp);
void block(FILE* fp,string first_str,int cx0,int tx0);
void condecl(FILE* fp);
void vardecl(FILE* fp);
void proc(FILE* fp);
void body(FILE* fp);
void statement(FILE* fp,string first_str);
void lexp(FILE* fp,string first_str);
void exp(FILE* fp,string first_str);
void term(FILE* fp,string first_str);
void factor(FILE* fp,string first_str);
void gotoblock(FILE* fp,string str);
void gotovardecl(FILE* fp,string str);
void gotoproc(FILE* fp,string str);
void gotobody(FILE* fp,string str);
void recyclestatement(FILE* fp,string str);
int find(string ident);
enum object{constant,variable,procedure};
enum fct{lit,opr,lod,sto,cal,inte,jmp,jpc};
int gen(enum fct x,int y,int z);
void enter(enum object k);
/*
opr,0,0 释放数据段指令
opr,0,1 取负指令
opr,0,2 加法指令
opr,0,3 减法指令
opr,0,4 乘法指令
opr,0,5 除法指令
opr,0,6 odd指令
opr,0,8 等于指令
opr,0,9 不等于指令
opr,0,10 小于指令
opr,0,11 小于等于指令
opr,0,12 大于指令
opr,0,13 大于等于指令
opr,0,14 输出指令
opr,0,15 输出换行
opr,0,16 输入指令
*/
struct instruction{
	enum fct f;
	int l;
	int a;
};
string ident;
int number;
int cx=0;
int lev=0;
int tx=0;
int dx;
struct instruction code[500];
struct tablestruct{
	string name;
	enum object kind;
	int val;
	int level;
	int adr;
	int size;
}table[100];
string op;
bool keyword(string str)
{
	if (str == "program")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "const")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "var")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "procedure")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "begin")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "end")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "if")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "then")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "else")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "while")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "do")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "call")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "read")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "write")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else if (str == "odd")
		//cout << "<" << str << "sym," << str << ">" << endl;
		return 1;
	else //cout << "<id," << str << ">" << endl;
		return 0;
}
string getsym(FILE* fp)
{
	string str;
	static int row = 1, col = 0;

	while (!feof(fp))
	{
		char ch = fgetc(fp);
		col++;
		if(ch==EOF)
			return"stop";
		if (ch == ' ')
			continue;
		else if (ch == '\n')
		{
			
			row++;
			col = 0;
			continue;
		}
		else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			str = "";
			str += ch;
			ch = fgetc(fp);
			col++;
			while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
			{
				str += ch;
				ch = fgetc(fp);
				col++;
			}
			fseek(fp, -1, 1);
			col--;
			if(keyword(str)==1)
				return str;
			else
			{
				ident=str;
				return "id";
			}
			
			break;
		}
		else if (ch >= '0' && ch <= '9')
		{
			int num;
			char str1[10];
			 str = "";
			str += ch;
			ch = fgetc(fp);
			col++;
			while (ch >= '0' && ch <= '9')
			{
				str += ch;
				ch = fgetc(fp);
				col++;
			}
			fseek(fp, -1, 1);
			col--;
			for(int i=0;i<10;i++)
			{
				str1[i]=str[i];
			}
			num=atoi(str1);
			number=num;
			return "num";
			//cout << "<num," << str << ">" << endl;
			break;
		}
		else if (ch == '+' || ch == '-')
		{
			op=ch;
			return "aop";
			//cout << "<aop," << ch << ">" << endl;
			break;
		}
		else if (ch == '*' || ch == '/')
		{
			op=ch;
			return "mop";
			//cout << "<mop," << ch << ">" << endl;
			break;
		}
		else if (ch == '=')
		{
			op=ch;
			return "lop";
			//cout << "<equal," << ch << ">" << endl;
			break;
		}
		else if (ch == ';')
		{
			return "semicolon";
			//cout << "<semicolon," << ch << ">" << endl;
			break;
		}
		else if (ch == ',')
		{
			return "comma";
			//cout << "<comma," << ch << ">" << endl;
			break;
		}
		else if (ch == '(')
		{
			return "lparen";
			//cout << "<lparen," << ch << ">" << endl;
			break;
		}
		else if (ch == ')')
		{
			return "rparen";
			//cout << "<rparen," << ch << ">" << endl;
			break;
		}
		else if (ch == '<')
		{
			char pre_ch = ch;
			str = "";
			str += pre_ch;
			ch = fgetc(fp);
			col++;
			if (ch == '>')
			{
				str += ch;
				op=str;
				return "lop";
				//str += ch;
				//cout << "<lop," << str << ">" << endl;
			break;
			}
			else if (ch == '=')
			{
				str += ch;
				op=str;
				return "lop";
				//str += ch;
				//cout << "<lop," << str << ">" << endl;
			break;
			}
			else
			{
				fseek(fp, -1, 1);
				col--;
				op=pre_ch;
				return "lop";
				//cout << "<lop," << pre_ch << ">" << endl;				
			break;
			}
		}
		else if (ch == '>')
		{
			char pre_ch = ch;
			str = "";
			str += pre_ch;
			ch = fgetc(fp);
			col++;
			if (ch == '=')
			{
				str += ch;
				op=str;
				return "lop";
				//str += ch;
				//cout << "<lop," << str << ">" << endl;
			break;
			}
			else
			{
				op=pre_ch;
				fseek(fp, -1, 1);
				col--;
				return "lop";
				//cout << "<lop," << pre_ch << ">" << endl;
			break;
			}
		}
		else if (ch == ':')
		{
			char pre_ch = ch;
			str = "";
			str += ch;
			ch = fgetc(fp);
			col++;
			if (ch == '=')
			{
				return "becomes";
				//str += ch;
				//cout << "<becomes," << str << ">" << endl;
			break;
			}
			else
			{
				//cout << "<lop," << pre_ch << ">" << endl;
				fseek(fp, -1, 1);
				col--;
				cout << row << " row " << col << " col " << " is an illegal character!" ;
				continue;
			}
		}
		else
		{
			cout << row << " row " << col << " col " << "is an illegal character!" ;
			continue;
		}
			
	}
}
int depth=0;
string next1[10000]="0";
int main()
{
	
	FILE* fp;
	fp = fopen("pascal.txt", "r");
	if (fp == NULL)
	{
		printf("Fail to open file!\n");
		exit(0);
	}
		string ch;
	/*while(!feof(fp))
	{
		ch= getsym(fp);
		cout<<ch<<endl;
		if(ch=="stop")
		break;
	}*/
	int row=1;
	prog(fp);
	printf("Compile completed!\n");
	cout<<"table:"<<endl<<"name kind        val  lev  adr  size"<<endl;
	for(int i=0;i<tx;i++)
	{
	cout<<table[i].name<<"    ";
	switch(table[i].kind)
	{
		case constant:printf("constant");break;
		case variable:printf("variable");break;
		case procedure:printf("procedure");break;
	}
	cout<<"    "<<table[i].val<<"    "<<table[i].level<<"    "<<table[i].adr<<"    "<<table[i].size<<endl;
	}
	cout<<"code:"<<endl;
	for(int i=0;i<cx;i++)
	{
		cout<<i<<":";
		switch(code[i].f)
		{
			case inte:printf("int");break;
			case lit:printf("lit");break;
			case opr:printf("opr");break;
			case lod:printf("lod");break;
			case sto:printf("sto");break;
			case cal:printf("cal");break;
			case jmp:printf("jmp");break;
			case jpc:printf("jpc");break;
		}
		cout<<"    "<<code[i].l<<"    "<<code[i].a<<endl;
	}
	system("pause");
}
void error(int n)
{
	switch(n){
		case 0:printf("error(0):Lack of program entry keyword 'program'. ");break;
		case 1:printf("error(1):Lack of program name. ");break;
		case 2:printf("error(2):Lack of semicolon. ");break;
		case 3:/*printf("error(3):The keyword 'const' or 'var' or 'procedure' is missing. ");*/break;
		case 4:printf("error(4):Lack of condecl name. ");break;
		case 5:printf("error(5):Character of becomes is missing or wrong.");break;
		case 6:printf("error(6):Condecl must be a number. ");break;
		case 7:printf("error(7):Lack of comma or semicolon. ");break;
		case 8:printf("error(8):Lack of procedure name. ");break;
		case 9:printf("error(9):Lack of lparen. ");break;
		case 10:printf("error(10):Lack of rparen. ");break;
		case 11:printf("error(11):Lack of body entry keyword 'begin'. ");break;
		case 12:printf("error(12):Lack of statement ");break;
		case 13:printf("error(13):Lack of body ending keyword 'end'. ");break;
		case 14:printf("error(14):Lack of a exp. ");break;
		case 15:printf("error(15):Lack of a term. ");break;
		case 16:printf("error(16):Lack of a factor. ");break;
		case 17:printf("error(17):Lack of a id. ");break;
		case 18:printf("error(18):Lack of body ending keyword 'end'. ");break;
		case 19:printf("error(19):Lack of a lexp. ");break;
		case 20:printf("error(20):Lack of keyword 'then'. ");break;
		case 21:printf("error(21):Lack of ketword 'do'. ");break;
		case 22:printf("error(22):Lack of a lop. ");break;
		case 23:printf("error(23):Lack of a aop. ");break;
		case 24:printf("error(24):The ident is undefined. ");break;
		case 25:printf("error(25):The ident is not a variable. ");break;
		case 26:printf("error(26):The ident needs a constant or variable. ");break;
		case 27:printf("error(27):After call needs a procedure. ");break;
	}
	/*system("pause");
	exit(0);*/


}
void prog(FILE* fp)
{
	string str=getsym(fp);
	if(str=="program")
	{
		str=getsym(fp);
		if(str=="id")
		{
			str=getsym(fp);
			if(str=="semicolon")
			{
				str=getsym(fp);
				if(str=="const"||str=="var"||str=="procedure"||str=="begin")
				{	int cx0=cx;
					int tx0=tx;
					dx=0;
					table[tx].adr=cx;
					block(fp,str,cx0,tx0);
				}
				else
				{
					error(3);
					gotoblock(fp,str);
				}
				
			}
			else
			{
				
				error(2);
				gotoblock(fp,str);
			}
			
		}
		else
		{
			error(1);
			gotoblock(fp,str);
		}
		
	}
	else
	{
		error(0);
		gotoblock(fp,str);
		return;
	}
	
}
void gotoblock(FILE* fp,string str)
{
	int cx0=0,tx0=0;
	if(str=="end")
	{
		
		return;
	}
	while(str!="end")
	{
		if(str=="const"||str=="var"||str=="procedure"||str=="begin")
		{
			block(fp,str,cx0,tx0);
			break;
		}
		else str=getsym(fp);
	}
}
int gen(enum fct x,int y,int z)
{
	if(cx>500)
	{
		printf("program is too long.");
		return -1;
	}
	code[cx].f=x;
	code[cx].l=y;
	code[cx].a=z;
	cx++;
	return 0;
}
void enter(enum object k)
{
	table[tx].name=ident;
	table[tx].kind=k;
	switch(k)
	{
		case constant:table[tx].val=number;break;
		case variable:
		table[tx].level=lev;
		table[tx].adr=dx;
		dx++;
		break;
		case procedure:table[tx].level=lev;break;
	}
	tx++;
}
int find(string ident)
{
	
	int i;
	i=tx;
	for(;i>=0;i--)
	{
		//if(lev<=table[i].level)
		{
			if(table[i].name==ident)
			return i;
		}
	}
	return -1;
}
void block(FILE* fp,string first_str,int cx0,int tx0)
{
	
	
	string str=first_str;
	if(str=="const")
	{
		condecl(fp);
		str=getsym(fp);
		if(str=="const"||str=="var"||str=="procedure"||str=="begin")
		{
			block(fp,str,cx0,tx0);
		}
		else
		{
			error(3);
			gotoblock(fp,str);
		}
	}
	else if(str=="var")
	{
		vardecl(fp);
		str=getsym(fp);
		if(str=="const"||str=="var"||str=="procedure"||str=="begin")
		{
			block(fp,str,cx0,tx0);
		}
			else
		{
			error(3);
			gotoblock(fp,str);
		}
	}
		
	else if(str=="procedure")
	{
		
		table[tx0].size=dx;
		
		proc(fp);
	
		if(next_str=="const"||next_str=="var"||next_str=="procedure"||next_str=="begin")
		{
			
			block(fp,next_str,cx0,tx0);
			
		}
			else
		{
			error(3);
			gotoblock(fp,next_str);
		}
	}
	else if(str=="begin"||next_str=="begin")
		{

			if(table[tx0].size==0)
			table[tx0].size=dx;
			code[table[tx0].adr].a=cx;
			table[tx0].adr=cx;
			cx0=cx;
			gen(inte,0,dx);
			body(fp);
			gen(opr,0,0);
		}
	else {
		error(11);
	}

	
}

void condecl(FILE* fp)
{
	string str;
	str=getsym(fp);
	if(str=="id")
	{
		str=getsym(fp);
		if(str=="becomes")
		{
			str=getsym(fp);
			if(str=="num")
			{
				enter(constant);
				str=getsym(fp);
				if(str=="comma")
					condecl(fp);
				else if(str=="semicolon")
					return;
				else
					error(7);
					gotovardecl(fp,str);
			}
			else
				error(6);
				gotovardecl(fp,str);
			
		}
		else
			error(5);
			gotovardecl(fp,str);
	}
	else
	{
		error(4);
		gotovardecl(fp,str);
	}
	
}
void gotovardecl(FILE* fp,string str)
{
	while(str!="stop")
	{
		if(str=="var")
		{
			vardecl(fp);
			break;
		}
		if(str=="procedure")
		{
			proc(fp);
			break;
		}
		if(str=="begin")
		{
			body(fp);
			break;
		}
		else str=getsym(fp);
	}
}
void gotoproc(FILE* fp,string str)
{
	while(str!="stop")
	{
		if(str=="procedure")
		{
			proc(fp);
			break;
		}
		if(str=="begin")
		{
			body(fp);
			break;
		}
		else str=getsym(fp);
	}
}
void gotobody(FILE* fp,string str)
{
	while(str!="stop")
	{
		if(str=="begin")
		{
			body(fp);
			break;
		}
		else str=getsym(fp);
	}
}
void vardecl(FILE* fp)
{
	string str;
	str=getsym(fp);
	if(str=="id")
			{
				enter(variable);
				str=getsym(fp);
				if(str=="comma")
					vardecl(fp);
				else if(str=="semicolon")
					return;
				else
					{	error(7);
						gotoproc(fp,str);
					}
			}
	else{
			error(17);
			gotoproc(fp,str);
	}
}
void proc(FILE* fp)
{
	string str;
	str=getsym(fp);
	if(str=="id")
	{
		int tx0=tx;
		enter(procedure);
		lev++;
		str=getsym(fp);
		if(str=="lparen")
		{
			str=getsym(fp);
			while(str=="id")
			{
				str=getsym(fp);
				if(str=="comma")
					str=getsym(fp);
				else break;
			}
			if(str=="rparen")
			{
				str=getsym(fp);
				if(str=="semicolon")
				{
					str=getsym(fp);
					if(str=="const"||str=="var"||str=="procedure"||str=="begin")
						{	int cx0=cx;
							
							dx=0;
							table[tx].adr=cx;
							block(fp,str,cx0,tx0);
							lev--;
						}
					str=getsym(fp);
					if(str=="semicolon")
					{
						str=getsym(fp);
						if(str=="procedure")
						{
							proc(fp);
						}
						else
						{
							next_str=str;
							return;
						} 
					}
					else 
					{
						error(2);
						gotobody(fp,str);
					}
				}
				else {
						error(2);
						gotobody(fp,str);
					}
			}
			else {
					error(10);
					gotobody(fp,str);
				}
		}
		else {
				error(9);
				gotobody(fp,str);
			}
	}
	else	
		{
			error(8);
			gotobody(fp,str);
		}
	
	
}

void body(FILE* fp)
{
	string str;
	depth++;
		str=getsym(fp);
		if(str=="id"||str=="if"||str=="while"||str=="call"||str=="read"||str=="write"||str=="begin")
			statement(fp,str);
		else {
			error(12);
			recyclestatement(fp,str);
		}
		str=next_str;
		
		//cout<<next_str;
		while(str=="semicolon")
		{		
			
			str=getsym(fp);
			if(str=="id"||str=="if"||str=="while"||str=="call"||str=="read"||str=="write"||str=="begin")
			{
				
				statement(fp,str);
				str=next_str;
			}
			else 
			{
				error(12);
				recyclestatement(fp,str);
				
			}
			
		}
		if (next1[depth+1]!="0")
		next1[depth]=getsym(fp);
		else
		next1[depth]=str;
		if(next1[depth]=="end")
		{
			
			//next1[depth]="0";
			depth--;
			return;
		}	
		else {error(13);}
	

}
void recyclestatement(FILE* fp,string str)
{
	if(str=="end")
	{
		
		return;
	}
	while(str!="end")
	{
		if(str=="semicolon")
		{
			str=getsym(fp);
			if(str=="id"||str=="if"||str=="while"||str=="call"||str=="read"||str=="write"||str=="begin")
			{
				statement(fp,str);
				str=next_str;
				break;
			}
			else if(str=="begin")
			{
				body(fp);
				break;
			}
			else continue;
		}
			else str=getsym(fp);
		
	}
}

void statement(FILE* fp,string first_str)
{
	int i;
	string str;
	if(first_str=="id")
	{
		i=find(ident);
		if(i==-1)
		{
			error(24);
		}
		else if(table[i].kind!=variable)
		{
			error(25);
			i=-1;
		}
		str=getsym(fp);
		if(str=="becomes")
		{
			str=getsym(fp);
			if(str=="aop"||str=="id"||str=="num"||str=="lparen")
			{
				exp(fp,str);
				if(i!=-1)
				{
					gen(sto,lev-table[i].level,table[i].adr);
				}
			}
			else {
				error(14);
				recyclestatement(fp,str);
			}
		}
		else {error(5);
		recyclestatement(fp,str);
		}
	}
	else if(first_str=="if")
	{
		int cx1,cx2;
		str=getsym(fp);
		if(str=="odd"||str=="aop"||str=="id"||str=="num"||str=="lparen")
		{
			lexp(fp,str);
		}
		else {error(19);
		recyclestatement(fp,str);
		}
		str=next_str;
		if(str=="then")
		{
			str=getsym(fp);
			cx1=cx;
			gen(jpc,0,0);
			if(str=="id"||str=="if"||str=="while"||str=="call"||str=="read"||str=="write")
			statement(fp,str);
			else if(str=="begin")
				body(fp);
			else {error(12);
			recyclestatement(fp,str);
			}
			code[cx1].a=cx;
		}
		else {error(20);
		recyclestatement(fp,str);
		}
		str=next_str;
		if(str=="else")
		{
			str=getsym(fp);
			if(str=="id"||str=="if"||str=="while"||str=="call"||str=="read"||str=="write")
			statement(fp,str);
			else if(str=="begin")
				body(fp);
			else {error(12);
			recyclestatement(fp,str);}
		}
		else 
		{
			next_str=str;
			return;
		}		
	}
	else if(first_str=="while")
	{
		int cx1,cx2;
		cx1=cx;
		str=getsym(fp);
		if(str=="odd"||str=="aop"||str=="id"||str=="num"||str=="lparen")
		{
			lexp(fp,str);
		}
		else {error(19);
		recyclestatement(fp,str);
		}
		cx2=cx;
		gen(jpc,0,0);
		str=next_str;
		if(str=="do")
		{
			str=getsym(fp);
			if(str=="id"||str=="if"||str=="while"||str=="call"||str=="read"||str=="write")
			statement(fp,str);
			else if(str=="begin")
				body(fp);
			else {error(12);recyclestatement(fp,str);}
			gen(jmp,0,cx1);
			code[cx2].a=cx;
		}
		else {error(21);recyclestatement(fp,str);}
	}
	else if(first_str=="call")
	{
		str=getsym(fp);
		if(str=="id")
		{
			i=find(ident);
			if(i==-1)
			{
				error(24);
			}
			else
			{
				if(table[i].kind==procedure)
				gen(cal,lev-table[i].level,table[i].adr);
				else
				{
					error(27);
				}
				
			}
			
			str=getsym(fp);
			if(str=="lparen")
			{
				str=getsym(fp);
		
				while(str=="aop"||str=="id"||str=="num"||str=="lparen")
				{
					exp(fp,str);
					str=next_str;
					if(str=="comma")
						continue;
					else break;
				}
				if(str=="rparen")
				{
					next_str=getsym(fp);
					return;
				}
				else {error(10);recyclestatement(fp,str);}
			}
			else {error(9);recyclestatement(fp,str);}
		}
		else {error(8);recyclestatement(fp,str);}
	}
	else if(first_str=="begin")
	{
		body(fp);
	}
	else if(first_str=="read")
	{
		string str;
		str=getsym(fp);
		if(str=="lparen")
		{
			str=getsym(fp);
			if(str=="id")
			{
				i=find(ident);
				if(i==-1)
				{
					error(24);
				}
				else 
				{
					gen(opr,0,16);
					gen(sto,lev-table[i].level,table[i].adr);
				}
				str=getsym(fp);
				while(str=="comma")
				{
					str=getsym(fp);
					if(str=="id")
						{
							i=find(ident);
							if(i==-1)
							{
								error(24);
							}
							else 
							{
								gen(opr,0,16);
								gen(sto,lev-table[i].level,table[i].adr);
							}
							str=getsym(fp);
						}
					else {error(17);recyclestatement(fp,str);}
				}
				if(str=="rparen")
				{
					next_str=getsym(fp);
					return;
				}
					
				else {error(10);
				recyclestatement(fp,str);
				}
			}
			else {error(17);
			recyclestatement(fp,str);
			}

		}
		else {error(9);
		recyclestatement(fp,str);
		}
	}
	else if(first_str=="write")
	{
	
		string str;
		str=getsym(fp);
			if(str=="lparen")
			{
				str=getsym(fp);
				if(str=="aop"||str=="id"||str=="num"||str=="lparen")
				{	
					exp(fp,str);
					gen(opr,0,14);
					str=next_str;
					while(str=="comma")
					{
						str=getsym(fp);
						if(str=="aop"||str=="id"||str=="num"||str=="lparen")
							{
								exp(fp,str);
								gen(opr,0,14);
								str=next_str;
							}
						else {error(14);
						recyclestatement(fp,str);}
					}
					if(str=="rparen")
					{
						next_str=getsym(fp);
						
						gen(opr,0,15);
						return;
					}
					else {error(10);recyclestatement(fp,str);}
				}
				else {error(14);recyclestatement(fp,str);}
			}
			else {error(9);recyclestatement(fp,str);}
	}
}
/*void gotoexp(FILE* fp,string str)
{
	while(str!="if"||str!="while"||str!="call"||str!="read"||str!="write"||str!="begin")
	{
		
		{
			lexp(fp,str);
			break;
		}
		else str=getsym(fp);
	}
}*/
void lexp(FILE* fp,string first_str)
{
	string str;
	string op1;
	if(first_str=="odd")
	{
		str=getsym(fp);
		if(str=="aop"||str=="id"||str=="num"||str=="lparen")
		{	
			exp(fp,str);
			gen(opr,0,6);
		}
		else {error(14);
		recyclestatement(fp,str);
		}
	}
	else 
	{
		exp(fp,first_str);
		str=next_str;
		if(str=="lop")
		{
			op1=op;
			str=getsym(fp);
			if(str=="aop"||str=="id"||str=="num"||str=="lparen")
			{	
				exp(fp,str);
			}
			else 
			{
				error(14);recyclestatement(fp,str);
			}
		}
		else 
		{
			error(22);recyclestatement(fp,str);
		}
		if(op1=="=")
		gen(opr,0,8);
		if(op1=="<>")
		gen(opr,0,9);
		if(op1=="<")
		gen(opr,0,10);
		if(op1=="<=")
		gen(opr,0,11);
		if(op1==">")
		gen(opr,0,12);
		if(op1==">=")
		gen(opr,0,13);
	}

}
void exp(FILE* fp,string first_str)
{
	string str,op1;
	op1=op;
	term(fp,first_str);
	if(op1=="-")
	gen(opr,0,1);
	if(next_str=="aop")
	{
		op1=op;
		str=getsym(fp);
		if(str=="id"||str=="num"||str=="lparen")
			{
				exp(fp,str);
				if(op1=="+")
				gen(opr,0,2);
				else
				{
					gen(opr,0,3);
				}
			}
		else {error(15);recyclestatement(fp,next_str);
		}
	}
	else 
	{		
		/*error(23);
		recyclestatement(fp,next_str);*/
		return;
	}
}

void term(FILE* fp,string first_str)
{
	string str,op1;
	factor(fp,first_str);
	str=getsym(fp);
	if(str=="mop")
	{
		op1=op;
		str=getsym(fp);
		if(str=="id"||str=="num"||str=="lparen")
		{
			term(fp,str);
			if(op1=="*")
			gen(opr,0,4);
			else
			{
				gen(opr,0,5);
			}
		}
		else {error(16);recyclestatement(fp,next_str);
		}

	}
	else 
	{
		next_str=str;
		return;
	}
}

void factor(FILE* fp,string str)
{
	int i;
	if(str=="id")
	{
		i=find(ident);
		if(i==-1)
		error(24);
		else
		{
			switch(table[i].kind)
			{
				case constant:
				gen(lit,0,table[i].val);break;
				case variable:
				gen(lod,lev-table[i].level,table[i].adr);break;
				case procedure:
				error(26);break;
			}
		}
		
	}
	else if(str=="num")
	{
		gen(lit,0,number);
		return;
	}
	else if(str=="lparen")
	{
		str=getsym(fp);
		if(str=="aop"||str=="id"||str=="num"||str=="lparen")
		{
			exp(fp,str);
		}
		else error(14);
		str=getsym(fp);
		if(str=="rparen")
			return;
		else {error(10);recyclestatement(fp,str);
		}
	}
}