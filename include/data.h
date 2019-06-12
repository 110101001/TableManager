#ifndef __DATA_H_
#define __DATA_H_

//data itself
class data{
	//friend class table;	
	public:
		data():isNum(0),str(0),strLen(0){}

		data(int val):isNum(1),isInt(1),intValue(val){}
		
		data(double val):isNum(1),isInt(0),floatValue(val){}

		data(char *_string,int _strLen);

		data(const class data &rhs);

		~data();

		bool operator<(const class data &rhs);
		bool operator==(const class data &rhs);
		bool operator>(const class data &rhs);
		void operator=(const class data &rhs);

		void editData(int val);

		void editData(double val);

		void editData(char *_string,int strlen);

		char *Str();

		int replace(data &source,data *to=0);//replace data with "to" if data.str=source.str, return 0 if match.
		int replace(data &source,data *to,int *fail);

		unsigned int hash();

		bool isEmpty();

		bool isNum;//Is it is a number?
	protected:
		union{
			struct {//It's a number!
				bool isInt;//Is it a interger?
				union{
					double floatValue;//It's a interger!
					int intValue;//It's not a interger!
				};
			};
			struct {//It's not a number!
				char *str;
				int strLen;
			};
		};
};

class data *readString(char *s);
void readString(data *pt,char *s);
int* failCalc(char *pat);

struct indexPointer;
typedef struct indexPointer indexPointer;

//A line of data
class line{
	public:
		line();
		line(class data **_datas,int _dataNum);
		line(int _capicity);
		line(int _size,class data repeat);
		~line();

		int insertData(int pos,class data* dataPt);
		int deleteData(int pos);

		data* operator[](int col);

		class line *next;
		class line *prev;
		indexPointer *parents;
	private:
		int size;
		int capicity;
		class data **datas;
};

//List of data in a table
typedef struct item{
		char *str;
		int strLen;
}item;

struct indexPointer{
	union{
		struct indexPointer *indexPt;
		line *linePt;
	};
	struct indexPointer *next;
	struct indexPointer *parents;
	int increase;
};

//The whole table
class table{
	/*This class is the table and all operations are based on it.
	 *Columns are stored by array because it won't insert of delete often.
	 *Rows are stored by linked list because we alway insert of delete a line of data.
	 *However, to improve random access performance, an index table can serve as a catagory.
	 *Titles are stored by linked list.
	 *
	 *Supported operations:
	 *1.Construct from file/Save to file.
	 *2.Insert/Delete.
	 *3.Search data with certain pattern.
	 *4.Sort lines of data.
	 *
	 */

	/*---------------------------------------------------------------------------
	 *Title: title0,title1,title2,title3
	 *---------------------------------------------------------------------------
	 *  index10, index00->line0: data0,data1,data2,data3
	 *	 |		 |		  |
	 *	 |		 |		  v
	 *	 |		 |		  line1: data0,data1,data2,data3
	 *	 |		 |		  |
	 *	 |		 |		  v
	 *	 |		 |		  line2: data0,data1,data2,data3
	 *	 |		 |		  |
	 *	 |		 |		  v
	 *	 |		 |		 ...
	 *   |		 |		  |
	 *   |		 |		  v
	 *	 |		index01->line100: data0,data1,data2,data3
	 *	 |		 |		  |
	 *	 |		 |		  v
	 *	 |		 |		 ...
	 *	 |		 |		  |
	 *	 |		 v		  v
	 *	 |		index02->line200: data0,data1,data2,data3
	 *	 |		 |		    |
	 *	 |		 v		    v
	 *	 |		...		   ...
	 *	 |		 |		    |
	 *	 v		 v		    v
	 *	index11->index0100->line10000: data0,data1,data2,data3
	 *----------------------------------------------------------------------------
	 */

	private:
		class line *lineHead;
		indexPointer **index;
		int indexDepth;

		void swapLine(line *a,line *b);
		void quickSort(line *begin,line *end,int position,bool assending);
	public:
		struct tableSize{
			int row;
			int col;
		} tableSize;
		class item **items;
		char *fileName;

		table():tableSize({0,0}),items(0),lineHead(0),index(0){}//construct a empty table
		table(char *_fileName);//construct a table by a file
		
		line *locateLine(int targetRow);//locate a line by its row, use indextable if it has
		void updateIndex();//adjust indexPointer

		int saveTable();//Save the table to it's original file, return 0 if success
		int saveTable(char *fileName);//Save the table to certain file,

		int editItem(int position, char *field);//edit an item, return 0 if success, return -1 if there are repeatation
		int searchItem(char *field);//search an item, return its position
		int insertItem(int position,class item *newItem,class data repeat);//insert a list of data, return 0 if success
		int insertData(class line* after, class line* newLine);//insert a list of data after "after", return 0 if success
		int insertData(int position, class line *newLine);//insert a line of data, return 0 if success

		int deleteItem(int position);//delete a list of data, return 0 if success
		int deleteData(int position);//delete a line of data, return 0 if success
		int deleteData(class line *del);//delete a line of data, return 0 if success

		int sortByItem(int position,bool asending);//sort lines of data

		class table *selectPart(int x,int y,int h,int w);//select part of the table and generate a new table.
		int searchItem(char *name,int strLen);//search item by name, return position
		class table *searchDataString(int itemPosition,class data source,int lowerRange=0,int upperRange=0);//search by Data, return a table including all search results
		class table *searchDataNum(int itemPosition,class data source,int relation,int lowerRange=0,int upperRange=0);//search by Data, return a table including all search results
		void replaceData(class data,class data newData,int itemPosition=-1,int lowerRange=0,int upperRange=0);//replace data, return a table including all search results

		unsigned int hash();
		class line *operator[](int row);
};




#endif
