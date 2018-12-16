-- phpMyAdmin SQL Dump
-- version 2.10.1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Mar 23, 2008 at 10:51 AM
-- Server version: 5.0.45
-- PHP Version: 5.2.5

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

-- 
-- Database: `livechat`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `message`
-- 

CREATE TABLE `message` (
  `id` decimal(20,6) NOT NULL,
  `contact_id` varchar(36) NOT NULL default '',
  `sender` varchar(255) NOT NULL,
  `message` text NOT NULL,
  `receiver` varchar(255) NOT NULL,
  `status` varchar(12) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 
-- Dumping data for table `message`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `operators`
-- 

CREATE TABLE `operators` (
  `id` int(11) NOT NULL auto_increment,
  `site_id` varchar(12) NOT NULL default '',
  `name` varchar(100) NOT NULL default '',
  `login` varchar(32) NOT NULL default '',
  `nickname` varchar(50) NOT NULL default '',
  `password` varchar(50) NOT NULL default '',
  `email` varchar(100) NOT NULL default '',
  `max_no_chats` varchar(20) NOT NULL default '',
  `accept_incoming` varchar(20) NOT NULL default '0',
  `cobrowse` varchar(20) NOT NULL default '',
  `isblocked` varchar(12) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- 
-- Dumping data for table `operators`
-- 

INSERT INTO `operators` (`id`, `site_id`, `name`, `login`, `nickname`, `password`, `email`, `max_no_chats`, `accept_incoming`, `cobrowse`, `isblocked`) VALUES 
(1, '12345', 'Suresh Gudivada', 'sureshg', 'suresh', 'da6df37c779ee033f5915c124182c7df', 'suresh.gudi@gmail.com', '', '0', '', '');

-- --------------------------------------------------------

-- 
-- Table structure for table `operatorspam`
-- 

CREATE TABLE `operatorspam` (
  `id` int(11) NOT NULL auto_increment,
  `operator_id` varchar(12) NOT NULL default '',
  `site_id` varchar(12) NOT NULL default '',
  `spamIP` varchar(20) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- 
-- Dumping data for table `operatorspam`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `operator_online`
-- 

CREATE TABLE `operator_online` (
  `operator_id` varchar(12) NOT NULL default '',
  `site_id` varchar(12) NOT NULL default '',
  `status` varchar(20) NOT NULL default '',
  `last_login` datetime default NULL,
  PRIMARY KEY  (`operator_id`,`site_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 
-- Dumping data for table `operator_online`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `site_details`
-- 

CREATE TABLE `site_details` (
  `id` int(11) NOT NULL default '0',
  `company` varchar(255) NOT NULL default '',
  `website` varchar(255) NOT NULL default '',
  `category` varchar(100) NOT NULL default '',
  `firstname` varchar(100) NOT NULL default '',
  `lastname` varchar(100) NOT NULL default '',
  `email` varchar(100) NOT NULL default '',
  `phone` varchar(32) NOT NULL default '',
  `country` varchar(60) NOT NULL default '',
  `password` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 
-- Dumping data for table `site_details`
-- 

INSERT INTO `site_details` (`id`, `company`, `website`, `category`, `firstname`, `lastname`, `email`, `phone`, `country`, `password`) VALUES 
(12345, 'RamGroup', 'http://www.naramala.com', 'Personal', 'Ramanaraya', 'N', 'rana.ram@gmail.com', '-', 'India', 'da6df37c779ee033f5915c124182c7df');

-- --------------------------------------------------------

-- 
-- Table structure for table `users`
-- 

CREATE TABLE `users` (
  `id` int(11) NOT NULL auto_increment,
  `username` varchar(32) NOT NULL default '',
  `password` varchar(32) NOT NULL default '',
  `user_level` varchar(10) NOT NULL default '',
  `status` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- 
-- Dumping data for table `users`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `visitors`
-- 

CREATE TABLE `visitors` (
  `id` varchar(255) NOT NULL,
  `name` varchar(32) NOT NULL default '',
  `country` varchar(50) NOT NULL default '',
  `city` varchar(50) NOT NULL default '',
  `organization` varchar(150) NOT NULL default '',
  `ISP` varchar(100) NOT NULL default '',
  `connectiontype` varchar(60) NOT NULL default '',
  `ip` varchar(50) NOT NULL default '',
  `host` varchar(100) NOT NULL default '',
  `state` varchar(100) NOT NULL default '',
  `email` varchar(120) NOT NULL default '',
  `currentpage` varchar(255) default NULL,
  `referrer` varchar(255) default NULL,
  `no_of_pages` int(11) default NULL,
  `cobrowse` varchar(12) default NULL,
  `lastvisit` datetime default NULL,
  `lastchat` datetime default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 
-- Dumping data for table `visitors`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `visitors_online`
-- 

CREATE TABLE `visitors_online` (
  `id` varchar(255) NOT NULL,
  `httpsession` varchar(128) NOT NULL,
  `ip` varchar(36) NOT NULL,
  `contact_id` varchar(36) NOT NULL,
  `site_id` varchar(12) NOT NULL default '',
  `operator_id` varchar(12) NOT NULL default '',
  `status` varchar(20) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- 
-- Dumping data for table `visitors_online`
-- 

