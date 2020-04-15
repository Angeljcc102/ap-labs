
int main(){

	int value1 = 10, value2 = 20;
	float floatVal = 15.3;
	infof("INFORMATION WITH VALUE: %d \n", value1);
	warnf("WARNING with value: %f \n", floatVal);
	errorf("ERROR with values: %d , %d \n", value1, value2);
	panicf("PANIC with values %d, %d \n", value1, value2);
    return 0;
}
