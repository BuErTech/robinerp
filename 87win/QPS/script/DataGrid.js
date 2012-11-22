/*==================================/
*
*���ܣ�
* ʵ�ִӿͻ�����DataGrid��
* ѡ��һ�л���е�Ч�� 
*
===================================*/

//�ڿͻ���ѡ��dataGrid�е�һ�У������кż�¼��txtSelectIndex����
function getDgdSelectIndex(txtSelectIndex)
{
	var dgdTable = event.srcElement;
	if (dgdTable == null) return;

	//���ݣ���ȡDataGrid�ڿͻ��˵�Table
	while (dgdTable.tagName != 'TABLE')
	{
		dgdTable = dgdTable.parentElement;
	}

	var curRow = event.srcElement;
	if  (curRow == null) return;
	
	//���ݣ���ȡ�ͻ���ѡ����
	while (curRow.tagName != 'TR')
	{
		curRow = curRow.parentElement;
	}
	
	//��¼�к�
	var oSelectIndex = document.getElementById(txtSelectIndex);

	for (i = 1; i < dgdTable.rows.length; i++)
	{
		if  (curRow == dgdTable.rows[i])
		{
			if (oSelectIndex != null)
				oSelectIndex.value = i - 1;
			dgdTable.rows[i].className = 'dgdSelectedItem';
		}
		else
		{
			dgdTable.rows[i].className = 'dgdItem';
		}
	}
}

//ѡ��DataGrid���У��������е�CheckBox��Ϊѡ��״̬����ȡ�����е�ѡ��״̬��
//lastSelectedRowIndex: ���ѡ�����ڵ�ǰҳ������
function checkDgdRow(lastSelectedRowIndex)
{
	var dgdTable = event.srcElement;
	if (dgdTable == null) return;

	//���ݣ���ȡDataGrid�ڿͻ��˵�Table
	while (dgdTable.tagName != 'TABLE')
	{
		dgdTable = dgdTable.parentElement;
	}

	var oCurRow = event.srcElement;
	if  (oCurRow == null) return;
			
	if (event.srcElement.tagName == 'INPUT')
		if (event.srcElement.type == 'checkbox')
			var srcElementIsCheckBox = true; //�¼�ԴΪCheckBox
			
	//���ݣ���ȡ�ͻ���ѡ����
	while (oCurRow.tagName != 'TR')
	{
		oCurRow = oCurRow.parentElement;
	}
			
	for (i = 1; i < dgdTable.rows.length; i++)
	{
		if  (oCurRow == dgdTable.rows[i])
		{
			//����ѡ����
			for (var j=0; j < oCurRow.all.length; j++)
			{
				//��ȡѡ���е�CheckBox
				if (oCurRow.all[j].tagName == 'INPUT')
					if (oCurRow.all[j].type == 'checkbox' && !oCurRow.all[j].disabled)
					{
						var oSelectedRowCbx = oCurRow.all[j];
							
						if (oSelectedRowCbx.checked)
						{
							if (!srcElementIsCheckBox)
							{
								oSelectedRowCbx.checked = false;
								dgdTable.rows[i].className = 'dgdItem';
							}
							else
								dgdTable.rows[i].className = 'dgdSelectedItem';
						}
						else 
						{
							if (!srcElementIsCheckBox)
							{
								oSelectedRowCbx.checked = true;
								dgdTable.rows[i].className = 'dgdSelectedItem';
							}
							else
								dgdTable.rows[i].className = 'dgdItem';
							
							//��¼���ѡ�����ڵ�ǰҳ������
							if (document.all[lastSelectedRowIndex] != null)
								document.all[lastSelectedRowIndex].value = i - 1;
						}
						break;
					}
			}
		}
	}
}