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

		void editData(int val);

		void editData(double val);

		void editData(char *_string,int strlen);

		char *Str();

		bool isEmpty();

	protected:
		bool isNum;//Is it is a number?
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

//A line of data
class line{
	public:
		line();
		line(class data **_datas,int _dataNum);
		line(int _capicity);
		line(int _size,class data repeat);
		~line();

		data* operator[](int col);

		class line *next;
		class line *prev;
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

typedef struct indexPointer{
	union{
		struct indexPointer *indexPt;
		line *linePt;
	};
	struct indexPointer *next;
	int increase;
}indexPointer;
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
	 *  index10<-index00->line0: data0,data1,data2,data3
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
		char *fileName;
	public:
		struct tableSize{
			int row;
			int col;
		} tableSize;
		class item *items;

		table():tableSize({0,0}),items(0),lineHead(0){}//construct a empty table
		table(char *_fileName);//construct a table by a file
		
		line *locateLine(int targetRow);//locate a line by its row, use indextable if it has
		void updateIndex();//adjust indexPointer

		int saveTable();//Save the table to it's original file, return 0 if success
		int saveTable(char *fileName);//Save the table to certain file,

		int insertItem(class item newItem);//insert a list of data, return 0 if success
		int insertData(int position, class line);//insert a line of data, return 0 if success

		int deleteItem(int position);//delete a list of data, return 0 if success
		int deleteData(int position);//delete a line of data, return 0 if success

		int sortByItem(int position,bool asending);//sort lines of data

		int searchItem(char *name,int strLen);//search item by name, return position
		class table *searchData(int itemPosition,class data,int lowerRange=0,int upperRange=0);//search by Data, return a table including all search results
		class table *replaceData(int itemPosition,class data,class newData,int lowerRange=0,int upperRange=0);//replace data, return a table including all search results

		class line *operator[](int row);
};




#endif
