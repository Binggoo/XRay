// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 IMAGEPROCESS_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// IMAGEPROCESS_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef IMAGEPROCESS_EXPORTS
#define IMAGEPROCESS_API __declspec(dllexport)
#else
#define IMAGEPROCESS_API __declspec(dllimport)
#endif

IMAGEPROCESS_API int GetImageBits(const char *type); // 获取图像位数
IMAGEPROCESS_API int GetImageBits(const HImage &Image);
IMAGEPROCESS_API HImage GammaImage(const HImage &Image,double gamma); // Gamma校正
// 比较2副图像
IMAGEPROCESS_API HRegion CheckDifference(const HImage& Image,const HImage& Pattern, const HTuple& Mode, const HTuple& DiffLowerBound, const HTuple& DiffUpperBound, const HTuple& GrayOffset, const HTuple& AddRow, const HTuple& AddCol);
// 比较2副图像，分析出亮的部分和暗的部分
IMAGEPROCESS_API HRegion CheckDifference(const HImage& Model,const HImage& Check,const HTuple& DiffLowerBound, const HTuple& DiffUpperBound,HRegion *Dark);
// 显示匹配结果
IMAGEPROCESS_API void DisplayShapMatchingResult(HWindow *hWindow,HShapeModel hShapModel,HTuple Color,HTuple Row,HTuple Column,HTuple Angle,HTuple ScaleR,HTuple ScaleC,double Scale);
// 图像格式转换
IMAGEPROCESS_API HImage ConvertImage(const HImage& Image,int depth,int channel);
// 显示信息
IMAGEPROCESS_API void DisplayMessage(HWindow *hWindow,const char *msg,int Font,HTuple Row,HTuple Column,HTuple Color,BOOL Box);
// 旋转图像
IMAGEPROCESS_API HImage RotateImage(const HImage &Image,double dbPhi);
// 翻转图像 0 - 水平翻转，1 - 垂直翻转， -1 - 水平垂直翻转
IMAGEPROCESS_API HImage FlipImage(const HImage &Image,int flip);
// 图像反相
IMAGEPROCESS_API HImage InvertImage(const HImage &Image);
// 均值滤波
IMAGEPROCESS_API HImage MeanImage(const HImage &Image,long lMaskWidth,long lMaskHeight);
// 中值滤波 0 - circle，1 - square
IMAGEPROCESS_API HImage MedianImage(const HImage &Image,int MaskType,long radius);
// 高斯滤波
IMAGEPROCESS_API HImage GaussImage(const HImage &Image,long size);
// 增强
IMAGEPROCESS_API HImage EmphasizeImage(const HImage &Image,long lMaskWidth,long lMaskHeight,double dbFactor);