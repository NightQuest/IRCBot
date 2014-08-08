DROP TABLE IF EXISTS `log`;
CREATE TABLE `log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `user` varchar(255) NOT NULL,
  `target` varchar(255) NOT NULL,
  `action` varchar(50) NOT NULL,
  `data` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) DEFAULT CHARSET=utf8;
