# Policy examples

Copyright (c) 2020-2021 [Antmicro](https://www.antmicro.com)

This repository shows how to use the [ros-tracking-nodes-tester-node project](https://github.com/antmicro/ros-tracking-nodes-tester-node).

Currently, it only contains a "random" policy that returns random bounding boxes.

## Testing the tracking and detection algorithms

The aim of the [ros-tracking-nodes-tester-node](https://github.com/antmicro/ros-tracking-nodes-tester-node) and [vot-report-generator](https://github.com/antmicro/ros-tracking-nodes-vot-report-generator) projects is to measure the quality of detection and tracking policies on video sequences.
The detection and tracking of objects in a video feed is an interesting use case that can involve various computer vision algorithms - detectors, trackers, object descriptors, classifiers and other algorithms.
There are also deep learning algorithms that perform both detection and tracking of objects.
The aggregation of predictions from those diverse algorithms can form a detection and tracking policy.

Such complex systems require Continuous Integration-driven testing to ensure that the quality of predictions improves over time and any issues with the predictions are detected as soon as possible.

This repository demonstrates how such detection and tracking systems implemented with [Robotic Operating System ROS](https://www.ros.org) can be tested using the [ros-tracking-nodes-teste-node project](https://github.com/antmicro/ros-tracking-nodes-tester-node).

## System requirements

The system requirements are as follows:

* ROS (at least ROS Noetic Ninjemys is suggested),
* OpenCV (at least 4.1.1 is suggested),
* `cv_bridge` ROS package,
* CMake (`>=2.8.3`)

## Usage

### Project preparation

Firstly, clone the repository along with the submodules:

```
git clone --recursive https://github.com/antmicro/ros-tracking-nodes-policy-examples.git policy-examples
cd policy-examples/
```

The submodules for the project are:

* [ROS stopwatch node](https://github.com/antmicro/ros-tracking-nodes-stopwatch) - this is a ROS utility for measuring the time between publishes between two topics (either already existing topics, or created using Tic Toc API),
* [ROS tracking nodes tester node](https://github.com/antmicro/ros-tracking-nodes-tester-node) - this is a ROS package for testing object detection and tracking policies on video sequences,
* [VOT report generator](https://github.com/antmicro/ros-tracking-nodes-vot-report-generator) - this is a Python tool that collects the data from the work of the above nodes and produces a quality tests report.

Secondly, build the project:

```
catkin_make
```

This will build the project and create `build/` and `devel/` directories - the built binaries will be stored in the `devel/` directory

Thirdly, set up the ROS environment:

```
source devel/setup.bash
```

### Testing the policy

The tester is meant for Single Object Tracking (the datasets and tracking models are usually named VOT - Video Object Tracking).
It requires input data for testing in the form of video sequences with annotations in the [ALOV dataset](http://alov300pp.joomlafree.it/dataset-resources.html) format.
The sequences for this tester can be created using [video2dataset tool](https://github.com/antmicro/video2dataset) - go to the repository for details on the annotations format and creation of dataset sequences.

The sample sequence is shown in the `sample-dataset` directory.

To begin, run roscore in a separate shell:
```
source devel/setup.bash
roscore
```
The `src/tracking_tester/test.py` script is used to run different policies with different video sequences.
It runs the `tracking_tester` application, which:

* reads frames and annotation files,
* advertises frames to `tracking_tester/frame` topic,
* subscribes to `tracking_tester/bbox` topic,
* collects processing time using `stopwatch` topics,
* compares received bounding boxes with ground truth and computes IoU for each frame.

The ROS node that implements detection and tracking policy should subscribe to the `tracking_tester/frame` topic, make predictions based on those frames and advertise bounding boxes to the `tracking_tester/bbox` topic.
The format of the bounding boxes is specified in the [optional_boox_msg message type](https://github.com/antmicro/ros-tracking-nodes-tester-node/blob/master/msg/optional_bbox_msg.msg).

The `src/tracking_tester/test.py` script also runs the policy based on the configuration file in a format specified in the [Automated tester script section](https://github.com/antmicro/ros-tracking-nodes-tester-node#script-and-config).
The [sample.config](./sample.config) file in this repository is the sample configuration file.
It tests the [`example_tracker`](./src/example_policy/src/example_tracker/main.cpp), which just returns a random bounding box.

To start testing, run:

```
mkdir outdir
python3  src/tracking_tester/test.py ./sample-dataset/sequence-1/ ./outdir --passes 5 --config-path ./sample.config
```

This will run the policy runtime 5 times and perform predictions on a given dataset sequence.
The tester will save CSV files with processing times and quality metrics in the `outdir` directory.

In the end, to generate the report, run:

```
export PYTHONPATH=./src/vot-report-generator/:$PYTHONPATH
python3 -m vot_report_generator.main ./outdir/ ./sample-dataset/ --output report
```

The report should be generated in the `report` directory.

### Report structure

The report is generated in the HTML format described in [Generated reports section for vot-report-generator](https://github.com/antmicro/ros-tracking-nodes-vot-report-generator#generated-reports).
