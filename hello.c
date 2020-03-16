/*
 * hello.c
 *
 *  Created on: Mar 11, 2020
 *      Author: jeken
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

/* Create a obj struct for this module  */
struct hello_obj{
	char* print_ctx;
	int s_idx;
	int len;
};

/***************************platform device*****************************************/

/* Set a string obj to the match table */
static const struct hello_obj hello_obj_data = {

		.print_ctx = "hello world",
		.s_idx = 0,
		.len = strlen("hello world"),

};

static const struct of_device_id of_hello_data[] = {
		{ .compatible = "hello", .data = &hello_obj_data, },
		{ .compatible = "hello1",},
		{},
};

MODULE_DEVICE_TABLE(of,of_hello_data);

/************************platform driver******************************************/

static void hello_printf(struct hello_obj *print_obj){
	//char* str;

	/* Check obj is normally */
	if(((void*)0 == print_obj) || (print_obj->len <= 0) ||
	   (print_obj->s_idx >= print_obj->len) || ((void*)0 == print_obj->print_ctx)){
	  printk("print_obj error\n");
	  return;
	}

	/* Check string is not null */
	if( (void*)0 == print_obj->print_ctx)
		goto error;

	printk("^-^:%s\n",print_obj->print_ctx);

	return;
error:
	printk("%s  string null\n",__func__);
}

static int hello_probe(struct platform_device *pdev){

	const struct hello_obj *of_data;

	/* Get the date of the match device to driver */
	of_data = of_device_get_match_data(&pdev->dev);

	if((void*)0 == of_data)
		printk("%s, get match data null\n",__func__);

    /* Handle obj to print */
	hello_printf(of_data);

    return 0;
}

static int hello_remove(struct platform_device *pdev){

	printk("^-^: call %s ",__func__);

	return 0;
}

static struct platform_driver hello_driver = {
		.driver = {
				.name="hello",
				.of_match_table = of_hello_data,
		},
		.probe = hello_probe,
		.remove = hello_remove,
};

module_platform_driver(hello_driver);

MODULE_DESCRIPTION("This module is a demo for platform device driver");
MODULE_AUTHOR("Jeken Zhuang (jekenzhuang@foxmail.com)");
MODULE_LICENSE("GPL v2");

