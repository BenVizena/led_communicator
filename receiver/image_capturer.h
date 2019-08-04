#pragma once

#include <stdio.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

class Image_Capturer
{
    public:
        struct Image_Time_Struct
        {
            cv::Mat mat;
            long long capture_time;
        };

        static void start_capturing_images(int period_in_millis);
        static void stop_capturing_images();
        static void offset_for_next_capture(int offset);
        static void start_processing_images(int period_in_millis);
        static bool has_dataset();

        static std::vector<cv::Mat> get_next_dataset();
        
    private:
        Image_Capturer();
        ~Image_Capturer();

        static std::vector<cv::Mat> image_vec;
        static int period; // millis
        static int period_offset; // millis
        static bool capturing;

        static void capture_image(cv::VideoCapture &cap);
        static bool setup_cap(cv::VideoCapture &cap);
        static bool teardown_cap(cv::VideoCapture &cap);
        static std::string process_image(cv::Mat mat);
        static std::vector<cv::Point2i> highlight_bright_pixels(cv::Mat &mat);
        static long long get_current_time();
        static std::vector<cv::Point2i> get_orientation_pixels(std::vector<cv::Point2i> bright_pixels, cv::Mat &mat);
        static void sort_pixels_left_to_right(std::vector<cv::Point2i> &pixels);
        static bool bright_pixel_exists_within_3_pixels(std::vector<cv::Point2i> pixels, cv::Point2i point);
        static cv::Point2i light_exists_above(std::vector<cv::Point2i> pixels, uint index);
        static bool this_is_a_bright_pixel(std::vector<cv::Point2i> pixels, cv::Point2i point);
        static void get_stacked_orientation_pixels(std::vector<cv::Point2i> bright_pixels, std::vector<cv::Point2i> &orientation_points);
        static void get_solo_orientation_pixel(std::vector<cv::Point2i> bright_pixels, std::vector<cv::Point2i> &orientation_points);
        static bool* get_data_light_array(std::vector<cv::Point2i> bright_pixels, std::vector<cv::Point2i> &orientation_points);
        static std::string convert_light_to_str(bool* data_lights);
        static bool data_equivalent(bool* a, bool* b);
};
