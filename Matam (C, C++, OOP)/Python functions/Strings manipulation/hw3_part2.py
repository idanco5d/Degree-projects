def get_palindrom_dict(str):
    palindrom_dict = {}
    str_len = len(str)
    for sub_str_size in range(1, str_len):
        current_position = 0
        current_size_palindrom_sub_strs = []
        while current_position + sub_str_size <= str_len:
            sub_str_to_check = str[current_position:current_position+sub_str_size]
            left_to_right_of_sub_str = ""
            right_to_left_of_sub_str = ""
            for char in sub_str_to_check:
                left_to_right_of_sub_str += char
            for char in reversed(sub_str_to_check):
                right_to_left_of_sub_str += char
            if left_to_right_of_sub_str == right_to_left_of_sub_str:
                current_size_palindrom_sub_strs.append(left_to_right_of_sub_str)
            current_position += 1
        if len(current_size_palindrom_sub_strs) > 0:
            palindrom_dict[sub_str_size] = current_size_palindrom_sub_strs
    return palindrom_dict


def check_match(str):
    str_odd = str[1::2]
    str_even = str[2::2]
    if len(str_odd) != len(str_even):
        return False
    improved_histogram_odd = [0]*256
    improved_histogram_even = [0]*256
    for e in str_odd:
        improved_histogram_odd[ord(e)] = improved_histogram_odd[ord(e)]+1
    for e in str_even:
        improved_histogram_even[ord(e)] = improved_histogram_even[ord(e)]+1
    improved_histogram_odd.sort()
    improved_histogram_even.sort()
    improved_histogram_odd.reverse()
    improved_histogram_even.reverse()
    for i in range(256):
        if improved_histogram_even[i] == 0:
            break
        if improved_histogram_odd[i] > improved_histogram_even[i]:
            return False
    return True


