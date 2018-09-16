<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="//tagfile">
	    <xsl:comment>This is automaticaly generated. Please do not edit!</xsl:comment>
		<xsl:element name="assistantconfig">
			<xsl:attribute name="version">3.2.0</xsl:attribute>
			<xsl:element name="DCF">
			    <xsl:attribute name="title">QicsTable documentation</xsl:attribute>
			    <xsl:attribute name="ref">./html/index.html</xsl:attribute>
			    <xsl:attribute name="imagedir">./html/</xsl:attribute>
			    <xsl:attribute name="htmldir">./html/</xsl:attribute>
				<xsl:call-template name="subSection"/>
		    <xsl:element name="section"> 
			<xsl:attribute name="title">Related pages</xsl:attribute>
			<xsl:attribute name="ref">./html/pages.html</xsl:attribute>
		    </xsl:element>
		    <xsl:element name="section"> 
			<xsl:attribute name="title">Qics files</xsl:attribute>
			<xsl:attribute name="ref">./html/files.html</xsl:attribute>
		    </xsl:element>
		
		</xsl:element>
    
	</xsl:element>
	</xsl:template>

<!--    <xsl:template match="//compound [ @kind = 'class' and not(@kind = 'file' )]" name="RootSection">
	<xsl:element name="section"> 
	    <xsl:attribute name="title">classes</xsl:attribute>
	    
	    <xsl:attribute name="ref">./html/<xsl:value-of select="filename" /></xsl:attribute>
	
	    <xsl:call-template name="subSection" />
	    
	</xsl:element>

    </xsl:template> -->
    
    <xsl:template match="//compound [ @kind = 'class' and not( @kind = 'file')]" name="subSection">
	
	<xsl:element name="section"> 
	    <xsl:attribute name="title"><xsl:value-of select="name" /> class reference</xsl:attribute>

	    <xsl:attribute name="ref">./html/<xsl:value-of select="filename" /></xsl:attribute>
	    
	    <xsl:apply-templates />
	</xsl:element>
	
    </xsl:template>
    
    <xsl:template match="//compound/member [ ((@kind = 'slot' and not(@virtualness) and not(@protection)) or (@kind = 'property' and not(@virtualness) and not(@protection)) or (@kind = 'function' and not(@virtualness) and not(@protection)) or (@kind = 'signal' and not(@virtualness) and not(@protection)) ) and not(@kind='file') ]" name="TST">
	<xsl:element name="keyword">
		
		<xsl:attribute name="ref">./html/<xsl:value-of select="anchorfile" />#<xsl:value-of select="anchor" /></xsl:attribute>
		
		<xsl:value-of select="name" />
	</xsl:element>

    </xsl:template>
    
    <xsl:template match="text()" />
</xsl:stylesheet>
