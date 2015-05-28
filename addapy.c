#include <Python.h>
#include <math.h>
#include "adda.h"


static PyObject *ClsbindingError;

static PyObject* addapy_system(PyObject *self, PyObject *args)
{
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;

	sts = system(command);

	return PyLong_FromLong(sts);
}

static PyObject *addapy_add(PyObject *self, PyObject *args)
{
	double a;
	double b;

	if (!PyArg_ParseTuple(args, "dd", &a, &b)) {
		return NULL;
	}
	return Py_BuildValue("d", a + b);
}

static PyObject *addapy_Initialize_adda(PyObject *self)
{
	int ret;
	ret = open_adda();

	if (ret == 0)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject *addapy_Destroy_adda(PyObject *self)
{
	int ret;
	ret = close_adda();

	if (ret == 0)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject *addapy_get_distance(PyObject *self, PyObject *args)
{
	int i = 0;
	char *kind;
	double ConstA;
	double ConstB;
	double ConstC;
	double result;

	if (!PyArg_ParseTuple(args, "sddd", &kind, &ConstA, &ConstB, &ConstC)) {
		return NULL;
	}

	result = get_distance(kind, ConstA, ConstB, ConstC);
	return Py_BuildValue("d", result);
};

static PyObject *addapy_set_led(PyObject *self, PyObject *args)
{
	char *kind;
	double distance;
	double ConstA;
	double ConstB;
	double ConstC;
	int ret;

	if (!PyArg_ParseTuple(args, "sdddd", &kind, &distance, &ConstA, &ConstB, &ConstC))
	{
		return NULL;
	}

	ret = light_call(kind, distance, ConstA, ConstB, ConstC);

	if (ret == 0)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
};

static PyObject *addapy_device_cleaning(PyObject *self)
{
	int ret;
	ret = device_cleaning();
	if (ret == 0)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
};

static PyObject *addapy_set_usb(PyObject *self, PyObject *args)
{
	int ret;
	char *on_off;
	if (!PyArg_ParseTuple(args, "s", &on_off))
		return NULL;

	ret = set_usb(on_off);
	if (ret == 0)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
};

static PyObject *addapy_get_temperature(PyObject *self, PyObject *args)
{
	double ConstA, ConstB;
	double temperature;
	if (!PyArg_ParseTuple(args, "dd", &ConstA, &ConstB))
		return NULL;
	temperature = get_temperature(ConstA, ConstB);

	return Py_BuildValue("d", temperature);
};

static PyObject *addapy_get_humidity(PyObject *self, PyObject *args)
{
	double ConstA, ConstB;
	double humidity;
	if (!PyArg_ParseTuple(args, "dd", &ConstA, &ConstB))
		return NULL;
	humidity = get_humidity(ConstA, ConstB);
	return Py_BuildValue("d", humidity);
};

static PyObject *addapy_get_illumination(PyObject *self, PyObject *args)
{
	double ConstA, ConstB, ConstC, ConstD;
	double illumination;
	if (!PyArg_ParseTuple(args, "dddd", &ConstA, &ConstB, &ConstC, &ConstD))
		return NULL;
	illumination = get_illumination(ConstA, ConstB, ConstC, ConstD);
	return Py_BuildValue("d", illumination);
};

static PyObject *addapy_get_flowmeter_signal(PyObject *self)
{
	int ret;
	ret = get_flowmeter_signal();
	return Py_BuildValue("i", ret);
}

static PyObject *addapy_flow_check(PyObject *self)
{
	int ret;
	ret = flow_check();
	return Py_BuildValue("i", ret);
}

static PyObject *addapy_flow_check_start(PyObject *self)
{	
	int ret;
	ret = flow_check_start();
	return Py_BuildValue("i", ret);
}

static PyObject *addapy_flow_check_stop(PyObject *self)
{
	int ret;
	ret = flow_check_stop();
	return Py_BuildValue("i", ret);
}

static PyObject *addapy_adjust_led_start(PyObject *self)
{
	int ret;
	ret = adjust_led_start();
	return Py_BuildValue("i", ret);
}

static PyObject *addapy_adjust_led_stop(PyObject *self)
{
	int ret;
	ret = adjust_led_stop();
	return Py_BuildValue("i", ret);
}

static PyObject *addapy_set_calibration_value(PyObject *self, PyObject *args)
{
	int i = 0;
	char *kind;
	double plDistConstA, plDistConstB, plDistConstC;
	double noplDistConstA, noplDistConstB, noplDistConstC;
	double plLedConstA, plLedConstB, plLedConstC;
	double noplLedConstA, noplLedConstB, noplLedConstC;
	double result;

	if (!PyArg_ParseTuple(args, "dddddddddddd", 
		&plDistConstA, &plDistConstB, &plDistConstC, 
		&noplDistConstA, &noplDistConstB, &noplDistConstC,
		&plLedConstA, &plLedConstB, &plLedConstC,
		&noplLedConstA, &noplLedConstB, &noplLedConstC )) 
	{
		return NULL;
	}

	set_calibration_value(plDistConstA, plDistConstB, plDistConstC,
								   noplDistConstA, noplDistConstB, noplDistConstC,
								   plLedConstA, plLedConstB, plLedConstC,
								   noplLedConstA, noplLedConstB, noplLedConstC);
	Py_RETURN_NONE;
};

static PyMethodDef AddaPyMethods[] = {
	{ "system", addapy_system, METH_VARARGS, "Execute shell command" },
	{ "add",  addapy_add, METH_VARARGS, "add two value" },
	{ "Initialize_adda",  addapy_Initialize_adda, METH_NOARGS, "initialize adda board" },
	{ "Destroy_adda",  addapy_Destroy_adda, METH_NOARGS, "destroy adda connection" },
	{ "device_cleaning",  addapy_device_cleaning, METH_NOARGS, "cleaning CLS device" },
	{ "get_distance",  addapy_get_distance, METH_VARARGS, "get PL filter distance sensor value" },
	{ "set_led",  addapy_set_led, METH_VARARGS, "set led output" },
	{ "set_usb",  addapy_set_usb, METH_VARARGS, "set usb on/off" },
	{ "get_temperature", addapy_get_temperature, METH_VARARGS, "read temperature reading from temperature sensor" },
	{ "get_humidity", addapy_get_humidity, METH_VARARGS, "read humidity reading from humidity sensor" },
	{ "get_illumination", addapy_get_illumination, METH_VARARGS, "read illumination reading from illumination sensor" },
	{ "get_flowmeter_signal",  addapy_get_flowmeter_signal, METH_NOARGS, "reading water flow signal" },
	{ "flow_check",  addapy_flow_check, METH_NOARGS, "300 ms flow signal reader" },
	{ "flow_check_start",  addapy_flow_check_start, METH_NOARGS, "300 ms flow signal reader" },
	{ "flow_check_stop",  addapy_flow_check_stop, METH_NOARGS, "300 ms flow signal reader" },
	{ "adjust_led_start",  addapy_adjust_led_start, METH_NOARGS, "300 ms flow signal reader" },
	{ "adjust_led_stop",  addapy_adjust_led_stop, METH_NOARGS, "300 ms flow signal reader" },
	{ "set_calibration_value",  addapy_set_calibration_value, METH_VARARGS, "set led output" },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef addapymodule = {
	PyModuleDef_HEAD_INIT,
	"addapy",
	NULL,
	-1,
	AddaPyMethods
};


PyMODINIT_FUNC
PyInit_addapy(void)
{
	return PyModule_Create(&addapymodule);
}

int main(int argc, wchar_t *argv[])
{
	PyImport_AppendInittab("addapy", PyInit_addapy);
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PyImport_ImportModule("addapy");

	return 0;
}