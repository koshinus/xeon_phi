void shift_right(struct_type main_array[4][4], struct_type &score)
{
    struct_type last_sum_index = 1;
    for(struct_type i = 0; i < ar_size; i++)
        for(struct_type j = 0; j < ar_size-1; j++)
        {
            if(main_array[i][j] == main_array[i][j+1] && j != last_sum_index)
            {
                last_sum_index = j+1;
                main_array[i][j] = 0;
                main_array[i][j+1] *= 2;
                score += main_array[i][j+1];
            }
        }
    for(struct_type i = 0; i < ar_size; i++)
    {
        struct_type j = 0;
        while (j != ar_size - 1)
        {
            if (main_array[i][j] != 0 && main_array[i][j + 1] == 0)
            {
                main_array[i][j + 1] = main_array[i][j];
                main_array[i][j] = 0;
                j = 0;
            }
            else j++;
        }
    }
}
