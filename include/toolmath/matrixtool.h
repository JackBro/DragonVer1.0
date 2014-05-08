#ifndef _MATRIXTOOLS_H_
#define _MATRIXTOOLS_H_


#ifndef TOOLMATH_DLL
	#define TOOLMATH_DLL  __declspec(dllimport)
#endif



class TOOLMATH_DLL CBaseMat
{
protected:
	virtual int		RevOrder(int *ary, int n);

public:
	void	PrintMatrix(int *blk, int height = 8, int width = 8);
	void	PrintMatrix(double *blk, int height = 8, int width = 8);
};

enum MATRIXREADMODEenum
{
	MTX_NOCHANGE,
	MTX_ADDHALF
};

#define TEMPLATE_MATRIX		template <class T1, class T2>
#define CMATRIXTOOL			CMatrixTool <T1, T2>

template <class T1 = int, class T2 = double>
class  CMatrixTool : public CBaseMat
{
public:
	CMatrixTool(int height = 0, int width = 0, int mode = MTX_NOCHANGE);
	virtual ~CMatrixTool();

public:
	virtual int		Read(T1 *buf, int len);
	virtual int		Write(T1 *buf, int len);

public:
	int		Contrary(void);
	int		DeTriangle(void);

public:
	void	SetSize(int height, int width);
	void	GetSize(int *height, int *width);

public:
	virtual void operator = (CMatrixTool &a);
	virtual void operator += (CMatrixTool &a);
	virtual void operator -= (CMatrixTool &a);
	virtual void operator *= (CMatrixTool &a);
	virtual void operator /= (CMatrixTool &a);

	virtual void operator = (T1 *a);
	virtual void operator += (T1 *a);
	virtual void operator -= (T1 *a);
	virtual void operator *= (T1 *a);
	virtual void operator /= (T1 *a);

	virtual void operator = (T1 a);
	virtual void operator += (T1 a);
	virtual void operator -= (T1 a);
	virtual void operator *= (T1 a);
	virtual void operator /= (T1 a);

public:
	void	MRotate(int rotate);
	void	Cap(T1 min, T1 max);

protected:
	void	Equ(CMatrixTool &a);
	void	Add(CMatrixTool &a);
	void	Sub(CMatrixTool &a);
	void	Mul(CMatrixTool &a);
	void	Div(CMatrixTool &a);

	void	Add(T1 *a);
	void	Sub(T1 *a);
	void	Mul(T1 *a);
	void	Div(T1 *a);

	void	Equ(T1 a);
	void	Add(T1 a);
	void	Sub(T1 a);
	void	Mul(T1 a);
	void	Div(T1 a);

protected:
	void	Clear(void);
	double	FindMaxUnit(int k);
	void	Exchange(int col1, int row1, int col2, int row2);

public:
	void	PrintMatrix(void);

private:
	T2		*m_buf;

private:
	int		m_width, m_height;
	int		*m_position;
	int		m_mode;
};


//
// class CMatrixTool
//
TEMPLATE_MATRIX
CMATRIXTOOL::CMatrixTool(int height, int width, int mode)
{
	this->m_buf = NULL;
	this->m_position = NULL;
	this->m_mode = mode;
	this->Clear();
	this->SetSize(height, width);
}

TEMPLATE_MATRIX
CMATRIXTOOL::~CMatrixTool(void)
{
	this->Clear();
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Clear(void)
{
	if(this->m_buf)
	{
		delete [](this->m_buf);
		this->m_buf = NULL;
		delete [](this->m_position);
		this->m_position = NULL;
	}
	this->m_width = 0;
	this->m_height = 0;
}

TEMPLATE_MATRIX
void CMATRIXTOOL::SetSize(int height, int width)
{
	int size;

	size = width * height;
	if( (this->m_width * this->m_height) < size )
	{
		this->Clear();
		this->m_buf = new T2[size];
		this->m_position = new int[(width > height) ? (width << 1) : (height << 1)];
	}
	if(this->m_buf)
	{
		this->m_width = width;
		this->m_height = height;
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::GetSize(int *height, int *width)
{
	*height = this->m_height;
	*width = this->m_width;
}

TEMPLATE_MATRIX
int	CMATRIXTOOL::Write(T1 *buf, int len)
{
	int i, j, pos = 0;

	for(i = 0; (i < this->m_height) && (pos < len); i++)
	{
		for(j = 0; (j < this->m_width) && (pos < len); j++, buf++, pos++)
			this->m_buf[pos] = (T2)*buf;
	}
	return pos;
}

TEMPLATE_MATRIX
int	CMATRIXTOOL::Read(T1 *buf, int len)
{
	int i, j, pos = 0;
	double gap = 0;

	if(this->m_mode == MTX_ADDHALF)
		gap = 0.5;

	for(i = 0; (i < this->m_height) && (pos < len); i++)
	{
		for(j = 0; (j < this->m_width) && (pos < len); j++, buf++, pos++)
		{
			if(this->m_buf[pos] > 0)
				*buf = (T1)(this->m_buf[pos] + gap);
			else
				*buf = (T1)(this->m_buf[pos] - gap);
		}
	}
	return pos;
}

//============================================
TEMPLATE_MATRIX
void CMATRIXTOOL::operator = (CMatrixTool &a)
{
	this->Equ(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator += (CMatrixTool &a)
{
	this->Add(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator -= (CMatrixTool &a)
{
	this->Sub(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator *= (CMatrixTool &a)
{
	this->Mul(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator /= (CMatrixTool &a)
{
	this->Div(a);
}
//===============================

TEMPLATE_MATRIX
void CMATRIXTOOL::operator = (T1 *a)
{
	this->Write(a, this->m_width * this->m_height);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator += (T1 *a)
{
	this->Add(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator -= (T1 *a)
{
	this->Sub(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator *= (T1 *a)
{
	this->Mul(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator /= (T1 *a)
{
	this->Div(a);
}

//====================================
TEMPLATE_MATRIX
void CMATRIXTOOL::operator = (T1 a)
{
	this->Equ(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator += (T1 a)
{
	this->Add(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator -= (T1 a)
{
	this->Sub(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator *= (T1 a)
{
	this->Mul(a);
}

TEMPLATE_MATRIX
void CMATRIXTOOL::operator /= (T1 a)
{
	this->Div(a);
}

//============================================
TEMPLATE_MATRIX
void CMATRIXTOOL::Equ(CMatrixTool &a)
{
	int i, j, pos = 0;

	for(i = 0; (i < this->m_height) && (i < a.m_height); i++)
	{
		for(j = 0; (j < this->m_width) && (j < a.m_width); j++, pos++)
			this->m_buf[pos] = (T2)a.m_buf[pos];
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Add(CMatrixTool &a)
{
	int i, j, pos = 0;

	for(i = 0; (i < this->m_height) && (i < a.m_height); i++)
	{
		for(j = 0; (j < this->m_width) && (j < a.m_width); j++, pos++)
			this->m_buf[pos] += (T2)a.m_buf[pos];
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Sub(CMatrixTool &a)
{
	int i, j, pos = 0;

	for(i = 0; (i < this->m_height) && (i < a.m_height); i++)
	{
		for(j = 0; (j < this->m_width) && (j < a.m_width); j++, pos++)
			this->m_buf[pos] -= (T2)a.m_buf[pos];
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Mul(CMatrixTool &a)
{
	int i, col, row, pos1, pos2, pos3 = 0;
	CMatrixTool <T1, T2> dest(this->m_height, a.m_width);

	for(col = 0, pos2 = 0; col < this->m_height; col++, pos2 += this->m_width)
	{
		for(row = 0; row < a.m_width; row++, pos3++)
		{
			dest.m_buf[pos3] = 0;
			for(i = 0, pos1 = 0; i < this->m_width; i++, pos1 += a.m_width)
				dest.m_buf[pos3] += this->m_buf[pos2+i] * a.m_buf[row+pos1];
		}
	}
	this->SetSize(this->m_height, a.m_width);
	*this = dest;
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Div(CMatrixTool &a)
{
	int i, j, pos = 0;
	T2 gap = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
		{
			if(this->m_mode == MTX_ADDHALF)
				gap = (T2)(a.m_buf[pos]-1);
			if(this->m_buf[pos] > 0)
				this->m_buf[pos] = (this->m_buf[pos] + gap) / a.m_buf[pos];
			if(this->m_buf[pos] < 0)
				this->m_buf[pos] = (this->m_buf[pos] - gap) / a.m_buf[pos];
		}
	}
}

//=================================
TEMPLATE_MATRIX
void CMATRIXTOOL::Add(T1 *a)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] += (T2)a[pos];
	}
}
TEMPLATE_MATRIX
void CMATRIXTOOL::Sub(T1 *a)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] -= (T2)a[pos];
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Mul(T1 *a)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] *= a[pos];
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Div(T1 *a)
{
	int i, j, pos = 0;
	T2 gap = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
		{
			if(this->m_mode == MTX_ADDHALF)
				gap = (T2)(a[pos] - 1);
			if(this->m_buf[pos] > 0)
				this->m_buf[pos] = (this->m_buf[pos] + gap) / (T2)a[pos];
			if(this->m_buf[pos] < 0)
				this->m_buf[pos] = (this->m_buf[pos] - gap) / (T2)a[pos];
		}
	}
}

//=================================
TEMPLATE_MATRIX
void CMATRIXTOOL::Equ(T1 a)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] = (T2)a;
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Add(T1 a)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] += a;
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Sub(T1 a)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] -= a;
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Mul(T1 a)
{
	int i, j, pos = 0;

	if(a == 0)
		a = 1;
	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
			this->m_buf[pos] *= a;
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Div(T1 a)
{
	int i, j, pos = 0;
	T2 gap = 0;

//	ASSERT(a);
	if(a == 0)
		a = 1;
	if(this->m_mode == MTX_ADDHALF)
		gap = (a-1);

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
		{
			if(this->m_buf[pos] > 0)
				this->m_buf[pos] = (this->m_buf[pos] + gap) / a;
			if(this->m_buf[pos] < 0)
				this->m_buf[pos] = (this->m_buf[pos] - gap) / a;
		}
	}
}
//===================================

TEMPLATE_MATRIX
int	CMATRIXTOOL::DeTriangle(void)
{
	int k, i, j, adr, adr1;

	for(k = 0, adr1 = 0; k < this->m_width-1; k++, adr1 += this->m_width)
	{
		if(this->FindMaxUnit(k) == 0)
			return 1;
		this->Exchange(k, k, this->m_position[k], this->m_position[k+this->m_width]);

		adr = adr1 + this->m_width;
		for(i = k+1; i < this->m_width; i++, adr += this->m_width)
		{
			this->m_buf[adr+k] /= this->m_buf[adr1+k];
			for(j = k+1; j < this->m_width; j++)
				this->m_buf[adr+j] -= this->m_buf[adr1+j] * this->m_buf[adr+k];
		}
	}
	for(k = this->m_width-2; k >= 0; k--)
		this->Exchange(this->m_position[k], this->m_position[k+this->m_width], k, k);
	return 0;
}

TEMPLATE_MATRIX
int	CMATRIXTOOL::Contrary(void)
{
	int k, i, j, adr, pos;

	if(this->m_width != this->m_height)
		return 1;

	for(k = 0, pos = 0; k < this->m_width; k++, pos += this->m_width)
	{
		if(this->FindMaxUnit(k) == 0)
			return 1;
		this->Exchange(k, k, this->m_position[k], this->m_position[k+this->m_width]);
		this->m_buf[pos + k] = 1 / this->m_buf[pos + k];
		for(i = 0, adr = k; i < this->m_width; i++, adr += this->m_width)
		{
			if(i == k)
				continue;
			this->m_buf[adr] *= 0 - this->m_buf[pos + k];
		}
		for(i = 0, adr = 0; i < this->m_width; i++, adr += this->m_width)
		{
			if(i == k)
				continue;
			for(j = 0; j < this->m_width; j++)
			{
				if(j == k)
					continue;
				this->m_buf[adr +j] += 	this->m_buf[adr + k] * this->m_buf[pos + j];
			}
		}
		for(i = 0; i < this->m_width; i++)
		{
			if(i == k)
				continue;
			this->m_buf[pos + i] *= this->m_buf[pos + k];
		}
	}
	for(k = this->m_width-1; k >= 0; k--)
		this->Exchange(this->m_position[k+this->m_width], this->m_position[k], k, k);
	return 0;
}

TEMPLATE_MATRIX
double CMATRIXTOOL::FindMaxUnit(int k)
{
	int i, j, adr = k * this->m_width;
	double val = 0, temp;

	for(i = k, adr = k * this->m_width; i < this->m_height; i++, adr += this->m_width)
	{
		for(j = k; j < this->m_width; j++)
		{
			temp = fabs(this->m_buf[adr+j]);
			if(temp > val)
			{
				val = temp;
				this->m_position[k] = i;
				this->m_position[k+this->m_width] = j;
			}
		}
	}
	return val;
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Exchange(int col1, int row1, int col2, int row2)
{
	int i, adr1, adr2;
	double val;

	if(col1 != col2)
	{
		for(i = 0, adr1 = col1 * this->m_width, adr2 = col2 * this->m_width; 
			i < this->m_width; i++, adr1++, adr2++)
		{
			val = this->m_buf[adr1];
			this->m_buf[adr1] = this->m_buf[adr2];
			this->m_buf[adr2] = (T2)val;
		}
	}
	if(row1 != row2)
	{
		for(i = 0, adr1 = row1, adr2 = row2; i < this->m_height; 
			i++, adr1 += this->m_width, adr2 += this->m_width)
		{
			val = this->m_buf[adr1];
			this->m_buf[adr1] = this->m_buf[adr2];
			this->m_buf[adr2] = (T2)val;
		}
	}
}

TEMPLATE_MATRIX
void CMATRIXTOOL::MRotate(int rotate)
{
	CBaseConvert tools;
	T2 *buf = NULL;
	int i, j, parm[4], pos1, pos2, pos = 0, size;

	size = this->m_width * this->m_height;
	buf = new T2[size];
	if(buf == NULL)
		return;
	tools.GetRotateParm(this->m_width, this->m_height, rotate, parm, 1);
	for(i = 0, pos1 = parm[0]; i < this->m_height; i++, pos1 += parm[1])
	{
		for(j = 0, pos2 = parm[2]; j < this->m_width; j++, pos2 += parm[3], pos++)
			buf[pos1+pos2] = this->m_buf[pos];
	}
	memcpy(this->m_buf, buf, size * sizeof(T2));
	delete []buf;
}

TEMPLATE_MATRIX
void CMATRIXTOOL::Cap(T1 min, T1 max)
{
	int i, j, pos = 0;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++, pos++)
		{
			if(this->m_buf[pos] < (T2)min)
				this->m_buf[pos] = (T2)min;
			if(this->m_buf[pos] > (T2)max)
				this->m_buf[pos] = (T2)max;
		}
	}
}



#define MATRIX_FNAME	"matrix.txt"

TEMPLATE_MATRIX
void CMATRIXTOOL::PrintMatrix(void)
{
	int i, j;
	CFileBin fb;

	fb.Open(MATRIX_FNAME, "at");
	fb.Printf("\n==========\ntable size: %d X %d\n", this->m_height, this->m_width);
	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++)
			fb.Printf("%d, ", (int)(this->m_buf[i * this->m_width + j] + 0.5));
		fb.Printf("\n");
	}
}


#endif


