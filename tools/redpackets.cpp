int GetDecimal(double num,int dec_quat)
{
	const double EPSILON = 0.00000001;
	int result = 0;
	if (num <= EPSILON && num >= -EPSILON)
	{
		return 0;
	}
	char format[12] = { 0 };
	char buf[64] = { 0 };
	sprintf(format, "%%.%df", dec_quat);
	
	sprintf(buf, format, num);
	std::string ns = buf;
	int pos = ns.find('.');
	std::string s = ns.substr(pos+1);
	result = std::atoi(s.c_str());
	return result;
}

double RandDouble(double min, double max)
{
	if (min == max)
	{
		return min;
	}
	const double EPSINON = 0.00000001;

	double d_div = (max-min);
	int i_div = (int)d_div;
	int div_dec = GetDec(d_div);

	double result = 0.0f;
	int rInteger = 0;
	if (i_div > 0)
	{
		rInteger = rand() % i_div;
	}
	double rDec = 0;
	if (div_dec == 0)
	{
		div_dec = 100;
	}
	
	rDec = (double)(rand() % div_dec) / 100.0f;
	result = (double)(rInteger + rDec);
	if (EPSINON >= result && result>-EPSINON)
	{
		result = min;
	}
	return result;
}


void RedPacket(double total, int quantity)
{
	int i = 0;
	double tmp = 0;
	srand((unsigned int)time(NULL));
	double min = 0.01;
	for (i = 0; i < quantity; ++i)
	{
		double packet = RandDouble(min, total);
		if (i == quantity - 1)
		{
			packet = total;
		}
		printf("第%d个: %.2f\n", i + 1, packet);
		tmp += packet;
		total -= packet;
	}
	std::cout << "Total:" << tmp << std::endl;
}
