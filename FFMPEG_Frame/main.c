//modified code from ffmpeg tutorial website

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include <stdio.h>


// compatibility with newer API
//#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
//#define av_frame_alloc avcodec_alloc_frame
//#define av_frame_free avcodec_free_frame
//#endif

//-----------------------------------------------------------------
//-----------------------------------------------------------------
void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;

  // Open file
  sprintf(szFilename, "frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;

  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

  // Close file
  fclose(pFile);
}
//-------------------------------------------------------------

void initializeFFMPEG () {
    // Register all formats and codecs
    av_register_all();
}
//-------------------------------------------------------------

int openVideo(const size_t *fNum, const char *vidName) {
    // Initalizing these to NULL prevents segfaults!
    AVFormatContext   *pFormatCtx = NULL;  //* used for frame
    int               videoStream;
    AVCodecContext    *pCodecCtxOrig = NULL;
    AVCodecContext    *pCodecCtx = NULL;
    AVCodec           *pCodec = NULL;
    AVFrame           *pFrame = NULL;
    AVFrame           *pFrameRGB = NULL;
    AVPacket          packet;
    int               frameFinished;
    int               numBytes;
    uint8_t           *buffer = NULL;
    struct SwsContext *sws_ctx = NULL;
    size_t i; //used to count the number of frames
    size_t j = 0; //used to count the position in the frame array fNum


    // Open video file
    if(avformat_open_input(&pFormatCtx, vidName, NULL, NULL)!=0)
      return -1; // Couldn't open file

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
      return -1; // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, vidName, 0);

    // Find the first video stream
    videoStream=-1;

    for(i=0; i<pFormatCtx->nb_streams; i++)
      if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
        videoStream=i;
        break;
      }
    if(videoStream==-1)
      return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;
    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if(pCodec==NULL) {
      fprintf(stderr, "Unsupported codec!\n");
      return -1; // Codec not found
    }
    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
      fprintf(stderr, "Couldn't copy codec context");
      return -1; // Error copying codec context
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
      return -1; // Could not open codec

    // Allocate video frame
    pFrame=av_frame_alloc();

    // Allocate an AVFrame structure
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB==NULL)
      return -1;

    // Determine required buffer size and allocate buffer
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                    pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));


    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
           pCodecCtx->width, pCodecCtx->height);

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
                 pCodecCtx->height,
                 pCodecCtx->pix_fmt,
                 pCodecCtx->width,
                 pCodecCtx->height,
                 PIX_FMT_RGB24,
                 SWS_BILINEAR,
                 NULL,
                 NULL,
                 NULL
                 );

    // Read frames
    i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0) {
      // Is this a packet from the video stream?
      if(packet.stream_index==videoStream) {
        // Decode video frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

        // Did we get a video frame?
        if(frameFinished) {
      // Convert the image from its native format to RGB
      sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
            pFrame->linesize, 0, pCodecCtx->height,
            pFrameRGB->data, pFrameRGB->linesize);

      // Save the frame to disk
      if(++i == fNum[j])
        SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
        j++;
        }
      }

      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);
    } //end of while

    // Free the RGB image
    av_free(buffer);
    av_frame_free(&pFrameRGB);

    // Free the YUV frame
    av_frame_free(&pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);

    // Close the video file
    avformat_close_input(&pFormatCtx);
    return 1;
}
//-----------------------------------------------------


//-----------------------------------------------------
//-----------------------------------------------------
int countFrames (const char *vidName) {
    // Initalizing these to NULL prevents segfaults!
    AVFormatContext   *pFormatCtx = NULL;  //* used for frame
    int               videoStream = -1; //first video stream
    AVCodecContext    *pCodecCtxOrig = NULL;
    AVCodecContext    *pCodecCtx = NULL;
    AVCodec           *pCodec = NULL;
    AVFrame           *pFrame = NULL;
    AVFrame           *pFrameRGB = NULL;
    AVPacket          packet;
    int               frameFinished;
    int               numBytes;
    uint8_t           *buffer = NULL;
    size_t nframe = 0; //used to count the number of frames


    //TO OPEN THE VIDEO, FIND A COMPATIBLE STREAM AND FIND APPROPRIATE CODEX
    //--------------------------------------------------------
    if(avformat_open_input(&pFormatCtx, vidName, NULL, NULL)!=0) { // Open video file
        fprintf(stderr, "No video found!\n");
        return -1; // if no video
    }
    if(avformat_find_stream_info(pFormatCtx, NULL)<0) {// Retrieve stream information
        fprintf(stderr, "No stream found!\n");
        return -1; // if no stream
    }
    size_t i;
    for(i=0; i<pFormatCtx->nb_streams; i++) { //find a compatible video stream
      if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
        videoStream=i;
        break;
      }
    }
    if(videoStream==-1) {//if there is no valid stream
     fprintf(stderr, "No valid stream!\n");
     return -1;
    }

    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig=pFormatCtx->streams[videoStream]->codec;
    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if(pCodec==NULL) {
      fprintf(stderr, "Unsupported codec!\n");
      return -1; // Codec not found
    }
    //--------------------------------------------------------

    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
      fprintf(stderr, "Couldn't copy codec context");
      return -1; // Error copying codec context
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
      fprintf(stderr, "Can not open codec!\n");
      return -1; // Could not open codec
    }

    pFrame=av_frame_alloc(); // Allocate video frame
    pFrameRGB=av_frame_alloc(); // Allocate an AVFrame structure

    // Determine required buffer size and allocate buffer
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                    pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
           pCodecCtx->width, pCodecCtx->height);

    // Read frames and count
    //--------------------------------------------------------
    while(av_read_frame(pFormatCtx, &packet)>=0) {
      if(packet.stream_index==videoStream) { // Is this a packet from the video stream?
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet); // Decode video frame
        if(frameFinished == 1) {  // Did we get a video frame?
                ++nframe;
        }
      }
      av_free_packet(&packet);
    } //end of while
    //--------------------------------------------------------

    //steps to free resources
    //-----------------------
    av_free(buffer);
    av_frame_free(&pFrameRGB);     // Free the RGB image
    av_frame_free(&pFrame);        // Free the YUV frame
    avcodec_close(pCodecCtx);      // Close the codecs
    avcodec_close(pCodecCtxOrig);  // Close the codecs
    avformat_close_input(&pFormatCtx);  // Close the video file
    //-----------------------

    return nframe;  //return the number of frames
}
//----------------------------------------------------

int main() {

  initializeFFMPEG();
  int nframe = countFrames ("videotest.mp4");
  printf("The number of frames is: %d", nframe);


  size_t frames[5] = {50, 500,1000, 1200, 2000};

  int i;
  for (i = 0; i < 5; i++){
     // openVideo(frames[i], "videotest.mp4");
  }

  return 0;
}
