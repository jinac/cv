#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;


int get_edge_blobs(Mat frame, Mat edges, int debug = 0) {
    cvtColor(frame, edges, CV_BGR2GRAY);
    GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
    Canny(edges, edges, 0, 30, 3);
    dilate(edges, edges, Mat(), Point(-1,-1), 2, 1, 1);

    if (debug) {
        imshow("edges debug", edges);
    }

    return 0;
}

int get_contours(Mat edges,
                 vector<vector<Point>> &contours,
                 vector<Vec4i> &hierarchy) {
    findContours(edges, contours, hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
                 Point(0, 0));

    return 0;
}

int compare_contours() {

}

int main(int argc, char * argv[]) {
    /*  Expected Arguments:
            filename
    */
    if (argc < 1) {
        printf("Usage is:   filename\n");
        return 1;
    }

    char * filename = argv[1];
    printf("%s\n", filename);
    // Start Video
    VideoCapture cap(filename);
    // VideoCapture cap(0);

    if (!cap.isOpened()) {
        return -1;
    }

    Mat frame, edges;
    vector<vector<Point>> contours[2];
    Mat edges[2];

    while (cap.read(frame)) { // continue while getting new frames to work
        edges[0] = new Mat;
        get_edge_blobs(frame, edges);
        get_contours(edges, contours);

        edges[1].release();
        edges[1] = edges[0];

        if (waitKey(30) >= 0) {
            break;
        }
    }
    return 0;
}
