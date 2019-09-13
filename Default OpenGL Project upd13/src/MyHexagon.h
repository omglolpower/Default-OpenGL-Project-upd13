
float positionsHex[12];
float l3;
float transfer;
void hexagon(float centerX, float centerY,
	int locationVar,
	float l1,
	float l2,
	float ratioVar,
	bool type, 
	float scale)
{
	l3 = (l2 - l1) * ratioVar;
	l1 = l1 * scale / ratioVar;
	l2 = l2 * scale / ratioVar;
	l3 = l3 * scale / ratioVar;

	if (type)
	{
		positionsHex[0] = centerX + l1;
		positionsHex[1] = centerY + l3;

		positionsHex[2] = centerX - l1;
		positionsHex[3] = centerY + l3;

		positionsHex[4] = centerX + l2;
		positionsHex[5] = centerY;

		positionsHex[6] = centerX - l2;
		positionsHex[7] = centerY;

		positionsHex[8] = centerX + l1;
		positionsHex[9] = centerY - l3;

		positionsHex[10] = centerX - l1;
		positionsHex[11] = centerY - l3;
	}
	else
	{
		l1 = l1 * ratioVar;
		l2 = l2 * ratioVar;
		l3 = l3 / ratioVar;
		positionsHex[0] = centerX + l3;
		positionsHex[1] = centerY + l1;

		positionsHex[2] = centerX + l3;
		positionsHex[3] = centerY - l1;

		positionsHex[4] = centerX;
		positionsHex[5] = centerY + l2;

		positionsHex[6] = centerX;
		positionsHex[7] = centerY - l2;

		positionsHex[8] = centerX - l3;
		positionsHex[9] = centerY + l1;

		positionsHex[10] = centerX - l3;
		positionsHex[11] = centerY - l1;

	}
	
}