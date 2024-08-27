#pragma once
class CLabelControler;

namespace LabelInitializer
{
	class IMCILabelInitializer
	{
	public:
		IMCILabelInitializer(){};
		virtual void Initialize(CLabelControler* pLabelControler) = 0;
	};


	class CLASS_InitLabelLay_0128: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

	class CLASS_InitLabelLay_0256: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

	class CLASS_InitLabelLay_0480: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};


	class CLASS_InitLabelLay_0512: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

	class CLASS_InitLabelLay_0640: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};


	class CLASS_InitLabelLay_0800: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

	class CLASS_InitLabelLay_1024: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};


	class CLASS_InitLabelLay_1440: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

	class CLASS_InitLabelLay_2048: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

	class CLASS_InitLabelLay_4096: public IMCILabelInitializer
	{
	public:
		virtual void Initialize(CLabelControler* pLabelControler);
	};

}

