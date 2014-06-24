/*
Tao Du
taodu@stanford.edu
April 8, 2014
*/
#ifndef _LIBPR_PRMATRIX2D_H_
#define _LIBPR_PRMATRIX2D_H_

#include "../prStd.h"
#include "../base/PRPixel.h"

//	this is a simple 2 dimensional matrix implementation. It supports unsigned char, int, double and PRVector3
//	and it includes:
//		-	an interface to read from/write into binary file
//		-	reset the elements
//		-	get/set a single element in the matrix
//		-	get the height/width of the matrix

//	the binary data file format
//	Byte 0:	the typeid of the matrix:
//		0	-	invalid type
//		1	-	char
//		2	-	bool
//		3	-	int
//		4	-	double
//		5	-	PRVector3
enum PR_DATA_TYPE	{PR_CHAR_TYPE = 1, PR_BOOL_TYPE, PR_INT_TYPE, PR_DOUBLE_TYPE, PR_PRVECTOR3_TYPE};
//	Byte 1 - Byte 4: height
//	Byte 5 - Byte 8: width
//	Byte 9: matrix data in row major order: i.e. [0][0], [0][1], [0][2], ...
//	the file extension is .prb (P_honeR_eflectometry B_inary file)

template <class DataType>
class PRMatrix2D
{
public:
	//	init the matrix with zero
	PRMatrix2D();
	//	init the matrix with specific height and width
	PRMatrix2D(int height, int width);
	//	init the matrix from a binary file
	PRMatrix2D(const std::string filename);
	//	copy constructor - deep copy required
	PRMatrix2D(const PRMatrix2D& m);
	//	deconstructor, free the memory
	~PRMatrix2D();

	//	overload operator
	//	assignment - deep copy required
	PRMatrix2D& operator=(const PRMatrix2D& m);
	//	()	-	 random access
	DataType& operator()(int height, int width);

	//	read data from file
	void readFromFile(const std::string filename);
	//	write data into file
	void writeIntoFile(const std::string filename) const;

	//	reset the element
	void reset(const DataType &d);

	//	get a single element
	DataType getData(int height, int width) const;
	//	interpolate between the matrix
	DataType interpData(double height, double width) const;
	DataType interpData(const PRPixel& p) const;

	//	set a single element
	void setData(int height, int width, const DataType& d);

	//	get the height of the matrix
	int getHeight()	const {return m_height;}
	//	get the width of the matrix
	int getWidth()	const {return m_width;}

private:
	//	allocate new memory
	//	this function assumes the pointer is NULL
	//	before calling the function
	void init(int height, int width);
	//	free the memory
	void destroy();
	//	the height and width of the matrix
	int m_height;
	int m_width;
	//	the data of the matrix
	DataType **m_data;
};

//	template class definition
//	constructor
template <class DataType>
PRMatrix2D<DataType>::PRMatrix2D()
{
	//	init height and width with zero
	m_height = m_width = 0;
	//	init the point to be null
	m_data = NULL;
}

//	constructor
template <class DataType>
PRMatrix2D<DataType>::PRMatrix2D(int height, int width)
{
	assert(height > 0 && width > 0);
	//	set m_data to NULL before calling init
	m_data = NULL;
	init(height, width);
}

//	constructor from a data file
template <class DataType>
PRMatrix2D<DataType>::PRMatrix2D(const std::string filename)
{
	//	init an empty matrix
	m_height = m_width = 0;
	m_data = NULL;
	//	read data from the file
	readFromFile(filename);
}

//	copy constructor
template <class DataType>
PRMatrix2D<DataType>::PRMatrix2D(const PRMatrix2D<DataType>& m)
{
	//	init an empty matrix
	m_height = m_width = 0;
	m_data = NULL;
	//	if necessary, resize the matrix
	if (m.m_height != m_height || m.m_width != m_width)
	{
		//	free the pointer
		destroy();
		//	the m_data pointer is guaranteed to be NULL
		//	also set the height and width
		init(m.m_height, m.m_width);
	}
	//	copy the data
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_data[i][j] = m.m_data[i][j];
}

//	deconstructor
template <class DataType>
PRMatrix2D<DataType>::~PRMatrix2D()
{
	destroy();
}
	
//	assignment - deep copy required
template <class DataType>
PRMatrix2D<DataType>& PRMatrix2D<DataType>::operator=(const PRMatrix2D<DataType>& m)
{
	if (m.m_height != m_height || m.m_width != m_width)
	{
		//	free the pointer
		destroy();
		//	the m_data pointer is guaranteed to be NULL
		//	also set the height and width
		init(m.m_height, m.m_width);
	}
	//	copy the data
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_data[i][j] = m.m_data[i][j];
	return (*this);
}

//	()	-	random access
template <class DataType>
DataType& PRMatrix2D<DataType>::operator()(int height, int width)
{
	//	don't accept out of bound range
	assert(height >= 0 && height < m_height);
	assert(width >= 0 && width < m_width);
	//	access the element
	//	return a reference
	return m_data[height][width];
}

//	read data from file
template <class DataType>
void PRMatrix2D<DataType>::readFromFile(const std::string filename)
{
	//	open the file
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	assert(!fin.fail());
	//	read data type
	char type; 
	fin.read(&type, sizeof(char));
	//	test whether the data type matches the matrix
	switch (type)
	{
	case PR_CHAR_TYPE:
		assert(typeid(DataType) == typeid(char));
		break;
	case PR_BOOL_TYPE:
		assert(typeid(DataType) == typeid(bool));
		break;
	case PR_INT_TYPE:
		assert(typeid(DataType) == typeid(int));
		break;
	case PR_DOUBLE_TYPE:
		assert(typeid(DataType) == typeid(double));
		break;
	case PR_PRVECTOR3_TYPE:
		assert(typeid(DataType) == typeid(PRVector3));
		break;
	default:
		//	invalid file!
		assert(false);
		return;
	}
	//	read height and width:
	int height, width;
	fin.read((char *)&height, sizeof(int));
	fin.read((char *)&width, sizeof(int));
	//	assert both are nonnegative!
	assert(height >= 0 && width >= 0);
	//	allocate the memory:
	if (m_height != height || m_width != width)
	{
		//	we need to reallocate the memory
		//	free the old memory and set m_data to be NULL
		destroy();
		//	reallocate the memory
		//	reset height and width
		init(height, width);
	}
	//	now if height * width is nonzero, then m_data != NULL and we read data from the file
	if (m_height * m_width != 0)
		fin.read((char *)m_data[0], sizeof(DataType) * m_height * m_width);
	//	close the file
	fin.close();
}

//	write data into file
template <class DataType>
void PRMatrix2D<DataType>::writeIntoFile(const std::string filename) const
{
	std::ofstream fout(filename, std::ios::out | std::ios::binary);
	//	write data type
	//	currently we only support char, bool, int, double and PRVector3
	char type = 0;
	//	what is the type of the matrix?
	if (typeid(DataType) == typeid(char))
		type = PR_CHAR_TYPE;
	else if (typeid(DataType) == typeid(bool))
		type = PR_BOOL_TYPE;
	else if (typeid(DataType) == typeid(int))
		type = PR_INT_TYPE;
	else if (typeid(DataType) == typeid(double))
		type = PR_DOUBLE_TYPE;
	else if (typeid(DataType) == typeid(PRVector3))
		type = PR_PRVECTOR3_TYPE;
	else
		assert(false);	//	invalid type!

	fout.write((char *)&type, sizeof(char));
	//	write the height and width
	fout.write((char *)&m_height, sizeof(int));
	fout.write((char *)&m_width, sizeof(int));
	//	write down the whole matrix in row-major order
	//	the memory layout in PRVector3 is
	//	x y z x y z x y z ...
	//	if the matrix is nonempty
	if (m_height * m_width != 0)
		fout.write((char *)m_data[0], sizeof(DataType) * m_height * m_width);
	//	close the file stream
	fout.close();
}

//	reset the element
template <class DataType>
void PRMatrix2D<DataType>::reset(const DataType &d)
{
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_data[i][j] = d;
}

//	get a single element
//	do not call this function when the matrix is empty (0x0 matrix)
template <class DataType>
DataType PRMatrix2D<DataType>::getData(int height, int width) const
{
	assert(height >= 0 && height < m_height);
	assert(width >= 0 && width < m_width);
	return m_data[height][width];
}

//	interpolate between the matrix
template <class DataType>
DataType PRMatrix2D<DataType>::interpData(double height, double width) const
{
	assert(height >= 0 && height <= m_height - 1);
	assert(width >= 0 && width <= m_width - 1);
	int h0 = (int)height, w0 = (int)width;
	int h1 = std::min(h0 + 1, m_height - 1), w1 = std::min(w0 + 1, m_width - 1);
	double dh = height - h0, dw = width - w0;
	double d00 = m_data[h0][w0];
	double d01 = m_data[h0][w1];
	double d10 = m_data[h1][w0];
	double d11 = m_data[h1][w1];
	return d00 * (1 - dh) * (1 - dw)
		+ d01 * (1 - dh) * dw
		+ d10 * dh * (1 - dw)
		+ d11 * dh * dw;
}

template <class DataType>
DataType PRMatrix2D<DataType>::interpData(const PRPixel& p) const
{
	return interpData(p.height, p.width);
}

//	set the element
//	do not call this function when the matrix is empty (0x0 matrix)
template <class DataType>
void PRMatrix2D<DataType>::setData(int height, int width, const DataType& d)
{
	assert(height >= 0 && height < m_height);
	assert(width >= 0 && width < m_width);
	m_data[height][width] = d;
}

//	private method
//	allocate the memory
template <class DataType>
void PRMatrix2D<DataType>::init(int height, int width)
{
	//	this function assumes the pointer is NULL
	//	before calling
	assert(m_data == NULL);
	//	set the height and width
	m_height = height;
	m_width = width;
	//	if the size are zero
	if (m_height * m_width == 0)
	{
		//	reset both = 0 for robustness
		m_height = m_width = 0;
	}
	else
	{
		//	allocate new memory
		m_data = new DataType*[m_height];
		m_data[0] = new DataType[m_height * m_width];
		for (int i = 0; i < m_height; i++)
			m_data[i] = m_data[0] + i * m_width;
	}
}

//	destroy function
//	free the memory
template <class DataType>
void PRMatrix2D<DataType>::destroy()
{
	//	free the memory
	if (m_data)
	{
		if (m_data[0])
		{
			delete []m_data[0];
			m_data[0] = NULL;
		}
		delete []m_data;
		m_data = NULL;
	}
	//	set height and width to be zero
	m_height = m_width = 0;
}
#endif