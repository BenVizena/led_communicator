#include "image_capturer.h"

using namespace cv; // TODO: make it to where I don't need this


std::vector<cv::Mat> Image_Capturer::image_vec = {};
int Image_Capturer::period = 0;
int Image_Capturer::period_offset = 0;
bool Image_Capturer::capturing = false;

cv::VideoCapture cap(0);
std::mutex vector_mutex;
std::mutex offset_mutex;

bool Image_Capturer::setup_cap(cv::VideoCapture &cap)
{
    if (!cap.isOpened())
    {
        std::cerr << "ERROR: Unable to open the camera" << std::endl;
        return false;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640 / 2);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480 / 2);

    cap.set(CV_CAP_PROP_FPS, 90);

    return true;
}

void Image_Capturer::capture_image(cv::VideoCapture &cap)
{
    cv::Mat this_mat;
    cap >> this_mat;

//    long long this_time = get_current_time(); 

//    Image_Capturer::Image_Time_Struct this_struct;
//    this_struct.mat = this_mat;
//    this_struct.capture_time = this_time;

    vector_mutex.lock();
    Image_Capturer::image_vec.push_back(this_mat);
    vector_mutex.unlock();
}

void Image_Capturer::offset_for_next_capture(int offset)
{
//    std::cout << "setting offset to " << offset << std::endl;
    offset_mutex.lock();
    Image_Capturer::period_offset = offset;
    offset_mutex.unlock();
}

std::vector<cv::Point2i> Image_Capturer::get_orientation_pixels(std::vector<cv::Point2i> bright_pixels, cv::Mat &mat)
{
    using namespace cv;
    using namespace std;

    vector<Point2i> orientation_points; // index 0 will be bottom light, index 1 will be top light, index 2 will be solo light

    get_stacked_orientation_pixels(bright_pixels, orientation_points);

    if (orientation_points.size() == 2)
    {
        get_solo_orientation_pixel(bright_pixels, orientation_points);
    }

    return orientation_points;
}


std::string Image_Capturer::process_image(Mat mat)
{
    using namespace cv;

    CV_Assert(mat.depth() == CV_8U);

    std::vector<cv::Point2i> bright_pixels_vec = highlight_bright_pixels(mat); // highlight bright pixels
    sort_pixels_left_to_right(bright_pixels_vec); // sort pixels left to right
    std::vector<cv::Point2i> orientation_points_vec = get_orientation_pixels(bright_pixels_vec, mat); // get orientation pixels
 
    bool* data_light_array;
    bool got_data_lights = false;
    if (orientation_points_vec.size() == 3)
    {
        data_light_array = get_data_light_array(bright_pixels_vec, orientation_points_vec); // get data lights
        got_data_lights = true;
    }

    std::string result = "";
    if (got_data_lights)
    {
        result = convert_light_to_str(data_light_array); // look up what character data_lights_array corresponds to
    }

//    if (result != "")
//    {
//        std::cout << "result: " << result << std::endl;
//    }
/*
    Vec3b color = {0, 0, 255}; // B G R values
    for (uint i = 0; i < orientation_points_vec.size(); i++)
    {
        mat.at<Vec3b>(Point(orientation_points_vec[i])) = color;
    }

    imshow("win2", mat);
*/
    return result;
}

void Image_Capturer::start_processing_images(int period_in_millis)
{ 
    using namespace cv;

    setup_cap(cap);

    Image_Capturer::period = period_in_millis;
    Image_Capturer::capturing = true;
    Image_Capturer::period_offset = 0;

    std::vector<cv::Mat> dataset;
    bool got_new_dataset = false;
    int new_line_counter = 0;

    while (Image_Capturer::capturing)
    {
        std::string res1 = "";
        std::string res2 = "";
        std::string res3 = "";
        std::string my_result = "";
        int my_offset = 0;

        if (Image_Capturer::has_dataset())
        {
            my_offset = 0;
            my_result = "";

            dataset = Image_Capturer::get_next_dataset();
            if (dataset.size() == 3)
            {
            got_new_dataset = true;

            res1 = Image_Capturer::process_image(dataset[0]);
            res2 = Image_Capturer::process_image(dataset[1]);
            res3 = Image_Capturer::process_image(dataset[2]);

            if (res1.length() > 0 || res2.length() > 0 || res3.length() > 0)
            {
                new_line_counter = 0;
         //       std::cout << "new char: \n";
         //       std::cout << "res1: " << res1 << "\n";
         //       std::cout << "res2: " << res2 << "\n";
         //       std::cout << "res3: " << res3 << "\n";

                if (res1 == res2 && res1 != res3)
                {
                    my_offset -= 5;
                    my_result = res1;
                }
                else if (res1 != res2 && res2 == res3)
                {
                    my_offset += 5;
                    my_result = res2;
                }
                else if (res1 != res2 && res1 != res3 && res2 != res3)
                {
            //        std::cout << "All three images voted differently!" << std::endl;
                    my_result = res2; // just pick the middle one...
                }
                else // unanimous vote
                {
                    my_result = res1;
                }

                std::cout << my_result;

                res1 = "";
                res2 = "";
                res3 = "";

                offset_for_next_capture(my_offset);
            }
            else if (new_line_counter == 10)
            {
                std::cout << "\n" << std::endl;
                new_line_counter++;
            }
            else if (new_line_counter < 10)
            {
                new_line_counter++;
            }
            }
        }

        if (!got_new_dataset)
        {
        }
        else
        {
            got_new_dataset = false;
            dataset = {};
        }
    }

    Image_Capturer::teardown_cap(cap);

    std::cout << "stopped processing images!" << std::endl;
}

std::vector<cv::Point2i> Image_Capturer::highlight_bright_pixels(cv::Mat &mat)
{
    using namespace cv;

    int channels = mat.channels();
    int nRows = mat.rows;
    int nCols = mat.cols;
    int i, j, B;
    
    std::vector<Point2i> bright_points;

    uchar* p;

    for (i = 0; i < nRows - 0; i+=1)
    {
        p = mat.ptr<uchar>(i);
        for (j = 0; j < nCols - 1; j+=1) // was +=2
        {
            B = 0;

            B = p[j * channels + 0];
                
            if (B > 240)
            {
                bright_points.push_back(Point2i(j, i));
            }

        }
    }
/*
    Vec3b color = {0, 0, 255}; // B G R values
    for (uint i = 0; i < bright_points.size(); i++)
    {
        mat.at<Vec3b>(Point(bright_points[i])) = color;
    }
*/
    return bright_points;
}

void Image_Capturer::start_capturing_images(int period_in_millis)
{
    long long start_time = get_current_time(); 
    // spin loop to make sure that the processing thread is going before we start capturing images
    // both threads stop doing things if this spin loop hasn't exited after 10 seconds.
    while (!Image_Capturer::capturing && get_current_time() - 10000 < start_time)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    long long timer1 = get_current_time();

    while (Image_Capturer::capturing)
    {
        Image_Capturer::capture_image(cap);

        std::this_thread::sleep_for(std::chrono::milliseconds(period / 3 - (get_current_time() - timer1) + Image_Capturer::period_offset));

        timer1 = get_current_time();
    }

    std::cout << "stopped capturing images!" << std::endl;
    Image_Capturer::teardown_cap(cap);
}

void Image_Capturer::stop_capturing_images()
{
    Image_Capturer::capturing = false;
}

bool Image_Capturer::teardown_cap(VideoCapture &cap)
{
    cap.release();
    destroyAllWindows();

    std::cout << "shutting down" << std::endl;

    return true;
}

bool Image_Capturer::has_dataset()
{
    if (Image_Capturer::image_vec.size() >= 3)
    {
        std::cout << "";
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<cv::Mat> Image_Capturer::get_next_dataset()
{
    std::vector<cv::Mat> return_vec{};

    if (Image_Capturer::has_dataset())
    {
//        std::cout << "hi mom" << std::endl;
        return_vec.push_back(Image_Capturer::image_vec[0]);
        return_vec.push_back(Image_Capturer::image_vec[1]);
        return_vec.push_back(Image_Capturer::image_vec[2]);

        vector_mutex.lock();
        Image_Capturer::image_vec.erase(Image_Capturer::image_vec.begin(), Image_Capturer::image_vec.begin() + 3);
        vector_mutex.unlock();
    }
    else
    {
       std::cerr << "tried to get dataset from vector with fewer than 3 elements!" << std::endl; 
    }

    return return_vec;
}

long long Image_Capturer::get_current_time()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Image_Capturer::sort_pixels_left_to_right(std::vector<cv::Point2i> &pixels)
{
    using namespace cv;
    using namespace std;

    vector<Point2i> sorted_bright_pixels;

    int index_of_most_leftward_pixel;
    int most_leftward_pixel;

    for (uint i = 0; i < pixels.size(); i++)
    {
        index_of_most_leftward_pixel = -1;
        most_leftward_pixel = 50000;

        for (uint j = 0; j < pixels.size(); j++)
        {
            if (pixels[j].x <= most_leftward_pixel)
            {
                index_of_most_leftward_pixel = j;
                most_leftward_pixel = pixels[j].x;
            }
        }
        sorted_bright_pixels.push_back(pixels[index_of_most_leftward_pixel]);
        pixels.erase(pixels.begin() + index_of_most_leftward_pixel);
        i--;
    }

    pixels = sorted_bright_pixels;
}

/*
    this function is used to try to determine that a given pixel (indicated by index) is not just a stray pixel but is really a light.
*/
bool Image_Capturer::bright_pixel_exists_within_3_pixels(std::vector<cv::Point2i> pixels, cv::Point2i point)
{
    for (uint i = 0; i < pixels.size(); i++)
    {
        if (std::abs(pixels[i].x - point.x) <= 3 && std::abs(pixels[i].y - point.y) <= 3)
        {
            if (pixels[i].x - point.x != 0 && pixels[i].y - point.y != 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool Image_Capturer::this_is_a_bright_pixel(std::vector<cv::Point2i> pixels, cv::Point2i point)
{
    for (uint i = 0; i < pixels.size(); i++)
    {
        if (point == pixels[i])
        {
            return true;
        }
    }

    return false;
}

/*
*
* finds and returns a pixel of the light that is above the point indicated by the (vector, index) given, if there is a light above.
*
*/
cv::Point2i Image_Capturer::light_exists_above(std::vector<cv::Point2i> pixels, uint index)
{
    // 1) throw out all pixels in vector that arent within 3 pixels of pixels[index].x or are below pixels[index].y
    // 2) find lowest y (highest pixel on the image) value of remaining pixels
    // 3) go up, pixel by pixel, to the lowest y value
    // 4) if there is a gap (no bright_pixel_exists_within_3_pixels) and then more pixels (bright_pixel_exists_within_3_pixels), return true

    int reference_x = pixels[index].x;
    int reference_y = pixels[index].y;

    cv::Point2i return_pixel {-1, -1};

    // step 1
    for (uint i = 0; i < pixels.size(); i++)
    {
        if (pixels[i].x - 3 > reference_x || pixels[i].y > reference_y)
        {
            pixels.erase(pixels.begin() + i);
        }
    }

    if (pixels.size() == 0)
    {
        return return_pixel;
    }

    // step 2
    int highest_pixel = 50000;
    for (uint i = 0; i < pixels.size(); i++)
    {
        if (pixels[i].y < highest_pixel)
        {
            highest_pixel = pixels[i].y;
        }
    }

    // step 3 and 4
    int current_y_val = reference_y - 1;
    bool found_gap = false;
    while (current_y_val >= highest_pixel)
    {
        if (this_is_a_bright_pixel(pixels, cv::Point2i(reference_x, current_y_val)))
        {
            if (found_gap == true)
            {
                return_pixel.x = reference_x;
                return_pixel.y = current_y_val;
                return return_pixel;
            }
        }
        else if (found_gap == false)
        {
            found_gap = true;
        }

        current_y_val--;
    }

    return return_pixel;
}

void Image_Capturer::get_stacked_orientation_pixels(std::vector<cv::Point2i> bright_pixels, std::vector<cv::Point2i> &orientation_points)
{
    using namespace cv;
    using namespace std;

    for (uint i = 0; i < bright_pixels.size(); i++)
    {
        Point2i temp_point = light_exists_above(bright_pixels, i);
        if (temp_point.x != -1)
        {
            orientation_points.push_back(bright_pixels[i]); // bottom light
            orientation_points.push_back(temp_point);       // top light
            break;
        }
    }
}

void Image_Capturer::get_solo_orientation_pixel(std::vector<cv::Point2i> bright_pixels, std::vector<cv::Point2i> &orientation_points)
{
    int bottom_light_y = orientation_points[0].y;
    int top_light_y = orientation_points[1].y;

    Point2i farthest_right_valid_point {-1, -1};

    for (uint i = 0; i < bright_pixels.size(); i++)
    {
        // + 5 and - 5 to give the array a little wiggle room to be off-level.
        if (bright_pixels[i].y + 5 > top_light_y && bright_pixels[i].y - 5 < bottom_light_y && bright_pixels[i].x > farthest_right_valid_point.x)
        {
            farthest_right_valid_point = bright_pixels[i];
        }
    }

    orientation_points.push_back(farthest_right_valid_point);
}

bool* Image_Capturer::get_data_light_array(std::vector<cv::Point2i> bright_pixels, std::vector<cv::Point2i> &orientation_points)
{
    // 1) find the centerpoint between the two stacked orientation lights
    // 2) find the distance from the center point to the solo orientation light
    // 3) with the above distance, we can determine where each data light should be.
    //      a) check for bright pixel where each light should be
    //      b) if bright pixel, the light is on.

    bool* light_is_on = new bool[6]; // there are 6 data lights

    int x_point_between_stacked_pixels = orientation_points[0].x;
    int y_point_between_stacked_pixels = (orientation_points[0].y + orientation_points[1].y) / 2;
    int distance_from_stacked_pixels_x_to_solo_pixel_x = orientation_points[2].x - x_point_between_stacked_pixels;
    int space_between_lights = distance_from_stacked_pixels_x_to_solo_pixel_x / 7; // there are 7 hops from stacked lights to solo light.
    float y_difference_between_lights = (float)(y_point_between_stacked_pixels - orientation_points[2].y) / 7;

    for (int i = 0; i < 6; i++)
    {
        cv::Point2i light_spot {x_point_between_stacked_pixels + space_between_lights * (i + 1), y_point_between_stacked_pixels - (int)(y_difference_between_lights * (i + 1))};
        orientation_points.push_back(light_spot);
        if (bright_pixel_exists_within_3_pixels(bright_pixels, light_spot))
        {
            light_is_on[i] = true;
        }
        else
        {
            light_is_on[i] = false;
        }
    }

    return light_is_on;
}

bool Image_Capturer::data_equivalent(bool* a, bool* b)
{
    bool result = true;
    for (int i = 0; i < 6; i++)
    {
        if (a[i] != b[i])
        {
            result = false;
        }
    }
    return result;
}

std::string Image_Capturer::convert_light_to_str(bool* data_lights)
{
    bool A[6] = {0,0,0,0,0,1};
    bool B[6] = {0,0,0,0,1,0};
    bool C[6] = {0,0,0,0,1,1};
    bool D[6] = {0,0,0,1,0,0};
    bool E[6] = {0,0,0,1,0,1};
    bool F[6] = {0,0,0,1,1,0};
    bool G[6] = {0,0,0,1,1,1};
    bool H[6] = {0,0,1,0,0,0};
    bool I[6] = {0,0,1,0,0,1};
    bool J[6] = {0,0,1,0,1,0};
    bool K[6] = {0,0,1,0,1,1};
    bool L[6] = {0,0,1,1,0,0};
    bool M[6] = {0,0,1,1,0,1};
    bool N[6] = {0,0,1,1,1,0};
    bool O[6] = {0,0,1,1,1,1};
    bool P[6] = {0,1,0,0,0,0};
    bool Q[6] = {0,1,0,0,0,1};
    bool R[6] = {0,1,0,0,1,0};
    bool S[6] = {0,1,0,0,1,1};
    bool T[6] = {0,1,0,1,0,0};
    bool U[6] = {0,1,0,1,0,1};
    bool V[6] = {0,1,0,1,1,0};
    bool W[6] = {0,1,0,1,1,1};
    bool X[6] = {0,1,1,0,0,0};
    bool Y[6] = {0,1,1,0,0,1};
    bool Z[6] = {0,1,1,0,1,0};
    bool SPACE[6] = {0,1,1,0,1,1};
    bool PERIOD[6] = {0,1,1,1,0,0};

    if (data_equivalent(data_lights, A))
    {
        return "A";
    }
    else if (data_equivalent(data_lights, B))
    {
        return "B";
    }
    else if (data_equivalent(data_lights, C))
    {
        return "C";
    }
    else if (data_equivalent(data_lights, D))
    {
        return "D";
    }
    else if (data_equivalent(data_lights, E))
    {
        return "E";
    }
    else if (data_equivalent(data_lights, F))
    {
        return "F";
    }
    else if (data_equivalent(data_lights, G))
    {
        return "G";
    }
    else if (data_equivalent(data_lights, H))
    {
        return "H";
    }
    else if (data_equivalent(data_lights, I))
    {
        return "I";
    }
    else if (data_equivalent(data_lights, J))
    {
        return "J";
    }
    else if (data_equivalent(data_lights, K))
    {
        return "K";
    }
    else if (data_equivalent(data_lights, L))
    {
        return "L";
    }
    else if (data_equivalent(data_lights, M))
    {
        return "M";
    }
    else if (data_equivalent(data_lights, N))
    {
        return "N";
    }
    else if (data_equivalent(data_lights, O))
    {
        return "O";
    }
    else if (data_equivalent(data_lights, P))
    {
        return "P";
    }
    else if (data_equivalent(data_lights, Q))
    {
        return "Q";
    }
    else if (data_equivalent(data_lights, R))
    {
        return "R";
    }
    else if (data_equivalent(data_lights, S))
    {
        return "S";
    }
    else if (data_equivalent(data_lights, T))
    {
        return "T";
    }
    else if (data_equivalent(data_lights, U))
    {
        return "U";
    }
    else if (data_equivalent(data_lights, V))
    {
        return "V";
    }
    else if (data_equivalent(data_lights, W))
    {
        return "W";
    }
    else if (data_equivalent(data_lights, X))
    {
        return "X";
    }
    else if (data_equivalent(data_lights, Y))
    {
        return "Y";
    }
    else if (data_equivalent(data_lights, Z))
    {
        return "Z";
    }
    else if (data_equivalent(data_lights, SPACE))
    {
        return " ";
    }
    else if (data_equivalent(data_lights, PERIOD))
    {
        return ".";
    }
    
    return "";
}
