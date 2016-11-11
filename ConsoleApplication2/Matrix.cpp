#include "Matrix.h"
#include <iostream>
#include <cmath>

using namespace std;
Matrix::Matrix(int m, int n)
{
	if (m < 0 || n < 0)
	{
		cout << "�����С����Ϊ��\n";
		return;
	}
	rowNum = m;
	colNum = n;
	item = new double[m*n];
	for (int i = 0; i < m*n; i++)
	{
		item[i] = 0;
	}
}

//Ҳ���ö�ά�����ʼ��
Matrix::Matrix(double* items, int m, int n)
{
	rowNum = m;
	colNum = n;
	item = items;
}
Matrix::Matrix(int n)
{
	rowNum = colNum = n;
	item = new double[n*n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				set(i, j, 1);
			else
				set(i, j, 0);
		}
	}
}
Matrix::Matrix(const Matrix &M)
{
	colNum = M.colNum;
	rowNum = M.rowNum;
	//���ﲻ�ܶ�ָ��ֱ�Ӹ�ֵ,���ƶ����桢ת�õȲ�����Ӱ��ԭ����
	item = new double[colNum*rowNum];
	for (int i = 0; i < colNum*rowNum; i++)
	{
		item[i] = M.item[i];
	}
}
Matrix::~Matrix()
{
}
void Matrix :: Matrix_free()
{
	if (item != nullptr)
		delete[] item;
}
double Matrix::get(int i, int j) const
{
	return item[i*colNum + j];
}
void Matrix::set(int i, int j, double value)
{
	item[i*colNum + j] = value;
}
void Matrix::RowSwap(int i, int j, double multiply)
{
	if (j == -1)
	{
		for (int k = 0; k < colNum; k++)
		{
			set(i, k, multiply*get(i, k));
		}
	}
	else
	{
		for (int k = 0; k < colNum; k++)
		{
			set(j, k, multiply*get(i, k) + get(j, k));
		}
	}
}
void Matrix::RowSwap(int i, int j)
{
	Matrix _copy = *this;
	for (int k = 0; k < colNum; k++)
	{
		double swap = _copy.get(j, k);
		set(j, k, _copy.get(i, k));
		set(i, k, swap);
	}
}
Matrix Matrix::Trans() const
{
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(j, i));
		}
	}
	_copy.rowNum = this->colNum;
	_copy.colNum = this->rowNum;
	return _copy;
}
int Matrix::getRowNum() const
{
	return rowNum;
}
int Matrix::getColNum() const
{
	return colNum;
}
ostream& operator <<(ostream &os, const Matrix &m)
{
	for (int i = 0; i < m.rowNum; i++)
	{
		for (int j = 0; j < m.colNum; j++)
		{
			os << m.get(i, j);
			if (j != m.colNum - 1) os << " ";
			else os << "\n";
		}
	}
	return os;
}
Matrix Matrix::operator +(const Matrix &m)
{
	if (m.colNum != colNum || m.rowNum != rowNum) return *this;
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(i, j) + m.get(i, j));
		}
	}
	return _copy;
}
Matrix Matrix::operator -(const Matrix &m)
{
	if (m.colNum != colNum || m.rowNum != rowNum) return *this;
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(i, j) - m.get(i, j));
		}
	}
	return _copy;
}
Matrix Matrix::operator *(const double f)
{
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(i, j)*f);
		}
	}
	return _copy;
}
Matrix Matrix::operator *(const Matrix &m)
{
	if (colNum != m.rowNum)
	{
		cout << "�޷���ˣ�";
		return *this;
	}
	Matrix _copy(rowNum, m.getColNum());
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < m.colNum; j++)
		{
			double sum = 0;
			for (int k = 0; k < m.rowNum; k++)
			{
				sum += get(i, k)*m.get(k, j);
			}
			_copy.set(i, j, sum);
		}
	}
	return _copy;
}
Matrix Matrix::Inverse()
{
	Matrix _copy = *this;
	//�任���
	Matrix result(colNum);
	if (colNum != rowNum)
	{
		cout << "���󲻿��棡" << endl;
		return *this;
	}
	for (int i = 0; i < rowNum; i++)
	{
		int MaxRow = i;
		//�����ҵ���i�еľ���ֵ���������������к͵�i�л���
		double max = abs(_copy.get(i, i));
		for (int j = i; j < colNum; j++)
		{
			if (abs(_copy.get(j, j))>max)
			{
				max = abs(_copy.get(j, j));
				MaxRow = j;
			}
		}
		//����j��i����
		if (MaxRow != i)
		{
			result.RowSwap(i, MaxRow);
			_copy.RowSwap(i, MaxRow);
		}
		//����i���������б任������һ����0Ԫ�ػ�Ϊ1
		double r = 1.0 / _copy.get(i, i);
		_copy.RowSwap(i, -1, r);
		result.RowSwap(i, -1, r);
		//��Ԫ
		for (int j = 0; j < rowNum; j++)
		{
			if (j == i) continue;
			r = -_copy.get(j, i);
			_copy.RowSwap(i, j, r);
			result.RowSwap(i, j, r);
		}
	}
	return result;
}