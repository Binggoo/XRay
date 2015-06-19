// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� IMAGEPROCESS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// IMAGEPROCESS_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef IMAGEPROCESS_EXPORTS
#define IMAGEPROCESS_API __declspec(dllexport)
#else
#define IMAGEPROCESS_API __declspec(dllimport)
#endif

IMAGEPROCESS_API int GetImageBits(const char *type); // ��ȡͼ��λ��
IMAGEPROCESS_API int GetImageBits(const HImage &Image);
IMAGEPROCESS_API HImage GammaImage(const HImage &Image,double gamma); // GammaУ��
// �Ƚ�2��ͼ��
IMAGEPROCESS_API HRegion CheckDifference(const HImage& Image,const HImage& Pattern, const HTuple& Mode, const HTuple& DiffLowerBound, const HTuple& DiffUpperBound, const HTuple& GrayOffset, const HTuple& AddRow, const HTuple& AddCol);
// �Ƚ�2��ͼ�񣬷��������Ĳ��ֺͰ��Ĳ���
IMAGEPROCESS_API HRegion CheckDifference(const HImage& Model,const HImage& Check,const HTuple& DiffLowerBound, const HTuple& DiffUpperBound,HRegion *Dark);
// ��ʾƥ����
IMAGEPROCESS_API void DisplayShapMatchingResult(HWindow *hWindow,HShapeModel hShapModel,HTuple Color,HTuple Row,HTuple Column,HTuple Angle,HTuple ScaleR,HTuple ScaleC,double Scale);
// ͼ���ʽת��
IMAGEPROCESS_API HImage ConvertImage(const HImage& Image,int depth,int channel);
// ��ʾ��Ϣ
IMAGEPROCESS_API void DisplayMessage(HWindow *hWindow,const char *msg,int Font,HTuple Row,HTuple Column,HTuple Color,BOOL Box);
// ��תͼ��
IMAGEPROCESS_API HImage RotateImage(const HImage &Image,double dbPhi);
// ��תͼ�� 0 - ˮƽ��ת��1 - ��ֱ��ת�� -1 - ˮƽ��ֱ��ת
IMAGEPROCESS_API HImage FlipImage(const HImage &Image,int flip);
// ͼ����
IMAGEPROCESS_API HImage InvertImage(const HImage &Image);
// ��ֵ�˲�
IMAGEPROCESS_API HImage MeanImage(const HImage &Image,long lMaskWidth,long lMaskHeight);
// ��ֵ�˲� 0 - circle��1 - square
IMAGEPROCESS_API HImage MedianImage(const HImage &Image,int MaskType,long radius);
// ��˹�˲�
IMAGEPROCESS_API HImage GaussImage(const HImage &Image,long size);
// ��ǿ
IMAGEPROCESS_API HImage EmphasizeImage(const HImage &Image,long lMaskWidth,long lMaskHeight,double dbFactor);