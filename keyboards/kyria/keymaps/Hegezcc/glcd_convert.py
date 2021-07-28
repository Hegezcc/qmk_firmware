#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

# Use https://javl.github.io/image2cpp/ (https://github.com/javl/image2cpp) for conversion to other way around

from PIL import Image

CHAR_HEIGHT = 8
CHAR_WIDTH = 6

class GlcdImage:
	width = 0
	height = 0

	data = None
	major_x = 0
	major_y = 0
	minor_x = 0
	minor_y = 0

	def __init__(self, width, height):
		self.width = width
		self.height = height

		self.data = bytearray(width * height * CHAR_WIDTH * CHAR_HEIGHT)

	def add_bulk(self, data):
		for i in data:
			for j in range(8):
				self.add_bit(bool(i >> j & 1))

	"""Data is a single bool for next pixel"""
	def add_bit(self, bit):
		self.data[(self.major_x * CHAR_WIDTH + self.minor_x) + (self.major_y * CHAR_HEIGHT + self.minor_y)*self.width*CHAR_WIDTH] = 255 if bit else 0

		# Advance next index
		if self.minor_y < CHAR_HEIGHT - 1:
			self.minor_y += 1
		else:
			self.minor_y = 0
			if self.minor_x < CHAR_WIDTH - 1:
				self.minor_x += 1
			else:
				self.minor_x = 0
				#print('CHAR: ', str(self.major_x).rjust(2), str(self.major_y).rjust(2))
				if self.major_x < self.width - 1:
					self.major_x += 1
				else:
					self.major_x = 0
					if self.major_y < self.height - 1:
						self.major_y += 1
					else:
						self.major_y = 0



def to():
	data = []

	# Get bytes
	with open('glcdfont.c') as f:
		is_block_comment = False
		is_data = False
		while True:
			l = f.readline().strip()
			
			if l.startswith('#') or l.startswith('//'):
				# Preprocessor instruction or line comment
				continue

			if (not is_block_comment) and '/*' in l:
				# Block comment start, process previous data
				l = l[:l.find('/*')]
				is_block_comment = True

			
			if is_block_comment:
				if '*/' in l:
					# Block comment end
					# Line is now the line without comment
					l = l[l.find('*/') + 2:]
					is_block_comment = False
				else:
					# Block comment continues
					continue

			if l.strip() == '':
				# Final empty check
				continue

			if ' char font[] ' in l:
				# Data starts from a char array "font", and then from the first "{" character
				l = l[l.find(' char font[] ') + 13:]
				l = l[l.find('{') + 1:]

				is_data = True

			if is_data:
				# Process data
				if '}' in l:
					# End of data
					l = l = l[:l.find('}')]
					is_data = False

				bs = l.split(',')

				for b in bs:
					b = b.strip().lower()

					if b == '':
						continue

					base = 10

					if b.startswith('0x'):
						base = 16

					data.append(int(b, base))

				if not is_data:
					# No more data
					break

	g = GlcdImage(32, len(data) // (32*6))
	g.add_bulk(data)
	im = Image.frombuffer('L', (CHAR_WIDTH * g.width, CHAR_HEIGHT * g.height), g.data)
	im.save("glcd.png", "PNG")

if __name__ == '__main__':
	to()