#include <iostream>
#include <vector>

std::vector<int> extractElements(const std::vector<int>& input,size_t max_elements)
{
    std::vector<int> result;
    size_t total_elements = input.size();

    if (total_elements <= max_elements) {
        // 小于或等于 max_elements 就全取出来
        result = input;
    } else {
        // 大于 max_elements 就均匀取出 max_elements 个，并确保包含第一,二个和最后一，二个
        // result.push_back(input[0]); // 添加第一项
        // result.push_back(input[1]); // 添加第二项

        size_t remaining_elements = max_elements - 1; // 去掉前两项和最后两项
        double step = static_cast<double>(total_elements - 1) / (remaining_elements - 1);

        for (size_t i = 0; i < remaining_elements; ++i) {
            size_t index = static_cast<size_t>(i * step);
            result.push_back(input[index]);
        }

        // result.push_back(input[total_elements - 2]); // 添加倒数第二项
        // result.push_back(input[total_elements - 1]); // 添加最后一项
    }

    return result;
}
 
int main(int argc, char** argv) 
{
    std::vector input={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};

    std::vector<int> result = extractElements(input,3);

    // 打印结果
    std::cout << "Extracted elements:" << std::endl;
    for (int elem : result) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}