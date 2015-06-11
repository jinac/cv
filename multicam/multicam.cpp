#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

#define MAX_NUM_CAM 8

int open_cameras(char ** cam_id_array, int num_cam,
                 VideoCapture * cam_array) {
    for (int i=0; i<num_cam; i++) {
        cam_array[i] = VideoCapture(atoi(cam_id_array[i]));
        if (!cam_array[i].isOpened()) {
            printf("Failed to open camera %d\n", i);
            return 1;
        }
    }
    return 0;
}

int get_frames(VideoCapture * cam_array, int num_cam,
               Mat * frames) {
    bool read_success = true;
    for (int i=0; i<num_cam; i++) {
        read_success = cam_array[i].read(frames[i]);
        if (!read_success) {
            printf("Failed to read frame on cam %d\n", i);
            return 1;
        }
    }
    return 0;
}

void display_frames(Mat * frames, int num_cam) {
    String title;
    for (int i=0; i<num_cam; i++) {
        title = "Camera ";
        title += to_string(i);
        imshow(title, frames[i]);
    }
}

int main(int argc, char * argv[]) {

    if (argc < 2 || argc > 9) {
        printf("Usage is:   ./multicam [cam_id ...]\n");
        return 1;
    }

    int cam_error;

    int num_cam = argc - 1;
    char ** cam_id_array = argv + 1;
    VideoCapture cam_array[MAX_NUM_CAM];
    cam_error = open_cameras(cam_id_array, num_cam, cam_array);

    if (cam_error) {
        return EXIT_FAILURE;
    }

    Mat frames[MAX_NUM_CAM];

    // first cycle
    cam_error = get_frames(cam_array, num_cam, frames);
    display_frames(frames, num_cam);

    while (!cam_error) { // continue while getting new frames to work
        cam_error = get_frames(cam_array, num_cam, frames);
        display_frames(frames, num_cam);

        if (waitKey(30) >= 0) {
            break;
        }
    }
    return EXIT_SUCCESS;
}
