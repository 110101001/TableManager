#ifndef __DATA_H_
#define __DATA_H_
//data itself
class data{
	friend class table;	
	public:
	data():isNum(1),isInt(1),intValue(0)
	{}
	data(int val):isNum(1),isInt(1),intValue(val)
	{}
	data(float val):isNum(1),isInt(0),floatValue(val)
	{}
	data(char *_string,int _strLen):isNum(0),string(_string),strLen(_strLen)
	{}
	~data();


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
			char *string;
			int strLen;
		};
	};
}data;

//List of data in a table
class item{

};

//A line of data
class line{
	private:
	int size;
	int capicity;
	data *datas;
	public:

};

//The whole table
class table{
	/*This class is the table and all operations are based on it.
	 *Columns are stored by array because it won't insert of delete often.
	 *Rows are stored by linked list because we alway insert of delete a line of data.
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
	 *line0: data0,data1,data2,data3
	 * |
	 * v
	 *line1: data0,data1,data2,data3
	 * |
	 * v
	 *line2: data0,data1,data2,data3
	 * |
	 * v
	 *line3: data0,data1,data2,data3
	 * |
	 * v
	 *line4: data0,data1,data2,data3
	 *----------------------------------------------------------------------------
	 */

	private:
		struct tableSize{
			int raw;
			int col;
		} tableSize;

	public:

		table();//construct a empty table
		table(char *fileName);//construct a table by a file

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
};




#endif
