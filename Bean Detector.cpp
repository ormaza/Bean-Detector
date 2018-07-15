#include <iostream>
#include <string> 
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  int nobjects = 1;
  
  CvPoint p;
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  threshold(image, image, 60, 255, CV_THRESH_BINARY);    

  if(!image.data)
  {
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  // achar os pontos diferentes de branco e pintar com uma cor diferente
  nobjects=0;
  for(int i=0; i<height; i++)
  {
    for(int j=0; j<width; j++)
    {
      if(image.at<uchar>(i,j) == 0)
      {   	        
	p.x=j;
	p.y=i;
	floodFill(image,p,nobjects++);        
      }
    }
  }
  
  // para cada cor ver quantos pixels tem desta cor e se for menor que 100 eh ruido
  int vetor[nobjects];
  int numPixelsN = 0;
  for (int n = 0; n < nobjects; n++)
  {
    for(int i=0; i<height; i++)
    {
      for(int j=0; j<width; j++)
      {
         if(image.at<uchar>(i,j) == n)
         {
            numPixelsN++;
         }
      }
    }
    vetor[n] = numPixelsN;
    numPixelsN = 0;
  }

  int numFeijoes = 0;
  // se ruido: apaga
  // senao: conta mais um feijao
  for (int n = 0; n < nobjects; n++)
  {
    if(vetor[n] < 1000)
    {      
      for(int i=0; i<height; i++)
      {
        for(int j=0; j<width; j++)
        {
           if(image.at<uchar>(i,j) == n)
           {
              image.at<uchar>(i,j) = 255;  
           }
        }
      }   
    }
    else
    {
      numFeijoes++;
    }    
  }
  
  // depois de tirar o ruido passa usa o floodfill denovo agora para 
  // comecar a detectar o tamanho dos feijoes
  for(int i=0; i<height; i++)
  {
    for(int j=0; j<width; j++)
    {
      if(image.at<uchar>(i,j) != 255)
      {
        image.at<uchar>(i,j) = 0;
      }
    }
  }

  p.x=0;
  p.y=0;

  int coresDosFeijoes = 0;
  for(int i=0; i<height; i++)
  {
    for(int j=0; j<width; j++)
    {
      if(image.at<uchar>(i,j) == 0)
      {         
        coresDosFeijoes++;
        p.x=j;
        p.y=i;
        floodFill(image,p,coresDosFeijoes);        
      }
    }
  }

  int tamanhoDosFeijoes[coresDosFeijoes];
  for (int n = 1; n <= coresDosFeijoes; n++)
  {
    for(int i=0; i<height; i++)
    {
      for(int j=0; j<width; j++)
      {
        if(image.at<uchar>(i,j) == n)
        { 
          tamanhoDosFeijoes[n]++;        
        }
      }
    }
  }

  for (int i = 0; i < coresDosFeijoes; ++i)
  {
     if (tamanhoDosFeijoes[i] > 99999)
     {
        std::string s = std::to_string(tamanhoDosFeijoes[i]);
        s = s.substr (0,5);
        tamanhoDosFeijoes[i] = std::stoi(s);
     }
     else if(tamanhoDosFeijoes[i] <= 0)
     {
        std::string s = std::to_string(tamanhoDosFeijoes[i]);
        s = s.substr (1,5);
        tamanhoDosFeijoes[i] = std::stoi(s);
     }
  }

  cout << "numero de feijoes: " << numFeijoes << endl;
  int somatorio = 0;
  for (int i = 1; i <= coresDosFeijoes; ++i)
  {
    somatorio += tamanhoDosFeijoes[i];
    cout << "feijao " << i << ": " << tamanhoDosFeijoes[i] << endl;
  }

  int media = somatorio/numFeijoes;
  cout << "tamanho medio dos feijoes: " << media << endl;

  float desvio = 0;
  cout << "Qual o valor em porcentagem do desvio da media para que o feijao ainda seja aceito?" << endl;
  cin >> desvio;

  int valorMin = media - (desvio/100)*(media);
  int valorMax = media + (desvio/100)*(media);

  cout << "Minimo: " << valorMin << endl << "Maximo: " << valorMax << endl;

  Mat resultado;
  resultado = imread(argv[1],1);
  int tamanho;
 
  for(int i=0; i<height; i++)
  {
    for(int j=0; j<width; j++)
    {
      if(image.at<uchar>(i,j) == 255)
      {
        resultado.at<cv::Vec3b>(i, j)[2] = 255;
        resultado.at<cv::Vec3b>(i, j)[1] = 255;
        resultado.at<cv::Vec3b>(i, j)[0] = 255;
      }
      else
      {
        tamanho = tamanhoDosFeijoes[image.at<uchar>(i,j)];
        if(tamanho <= valorMin || tamanho >= valorMax)
        { 
          resultado.at<cv::Vec3b>(i, j)[2] = 255;
          resultado.at<cv::Vec3b>(i, j)[1] = 0;
          resultado.at<cv::Vec3b>(i, j)[0] = 0;        
        }
        else
        {
          resultado.at<cv::Vec3b>(i, j)[2] = 0;
          resultado.at<cv::Vec3b>(i, j)[1] = 255;
          resultado.at<cv::Vec3b>(i, j)[0] = 0;  
        }
      }        
    }
  }
     
  imwrite("resultado.png", resultado);
  waitKey();
  return 0;
}
